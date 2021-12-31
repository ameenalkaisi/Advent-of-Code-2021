#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <bitset>

using namespace std;

struct header {
    int version, type_id;

    header(int version, int type_id) : version{version}, type_id{type_id} {}
};

struct packet {
    header packet_header;

    packet(header packet_header) : packet_header{packet_header} {}

    virtual int get_version_sum() {
        return 0;
    }
};

struct literal_value_packet : public packet {
    unsigned int number;

    literal_value_packet(header packet_header, unsigned int number) : packet::packet(packet_header), number{number} {}

    int get_version_sum() override {
        return packet_header.version;
    }
};

struct operator_packet : public packet {
    vector<packet*> sub_packets;
    int length_type_id;

    // intialize sub packets later
    operator_packet(header packet_header, int length_type_id, vector<packet*> sub_packets) : packet::packet(packet_header), length_type_id{length_type_id},
        sub_packets{sub_packets} {}

    bool is_total_length() const {
        return length_type_id == 0;
    }

    bool is_sub_packet_count() const {
        return length_type_id == 1;
    }

    int get_version_sum() override {
        int total = packet_header.version; 
        for(auto i : sub_packets)
            total += i->get_version_sum();

        return total;
    }
};

packet* parse_packet_(string bin_stream, int &index) {
    int prev = index;

    // get version and type id
    int version = static_cast<int>(bitset<3>{bin_stream.substr(index, 3)}.to_ullong());
    index += 3;

    int type_id = static_cast<int>(bitset<3>{bin_stream.substr(index, 3)}.to_ullong());
    index += 3;

    header packet_header{version, type_id};

    if(type_id == 4) {
        // parse the number
        unsigned int num = 0;
        while(true) {
            bool stop = bin_stream[index++] == '0';
            // read next 4 bits into the number
            for(int i = 0; i < 4; ++i)
                num = 2*num + bin_stream[index++] - '0';

            if(stop)
                break;
        }

        return new literal_value_packet{packet_header, num};
    } else {
        // parse everything about operator packet
        vector<packet*> subpackets;
        int len_type_id = bin_stream[index++] - '0';

        if(len_type_id == 0) {
            // get the length
            int total_subpacket_len = 0;
            for(int i = 0; i < 15; ++i)
                total_subpacket_len = 2*total_subpacket_len + bin_stream[index++] - '0';
            
            int index_before_packets = index;
            while(index < index_before_packets + total_subpacket_len) {
                packet *cur_subpacket = parse_packet_(bin_stream, index);
                subpackets.push_back(cur_subpacket);
            }
        } else {
            // get number of packets to read
            int total_subpackets = 0;
            for(int i = 0; i < 11; ++i)
                total_subpackets = 2*total_subpackets + bin_stream[index++] - '0';

            for(int i = 0; i < total_subpackets; ++i) {
                packet *cur_subpacket = parse_packet_(bin_stream, index);
                subpackets.push_back(cur_subpacket);
            }
        }

        // return this one
        return new operator_packet{packet_header, len_type_id, subpackets};
    }
}

packet* parse_packet(string bin_stream) {
    int index = 0;
    return parse_packet_(bin_stream, index);
}

int main(int argc, char **argv) {
    ifstream input{argv[1]};
    if(!input)
        exit(1);

    string in_stream;
    getline(input, in_stream);

    // convert every hex digit to binary
    string bin_stream;
    for(string::iterator i = in_stream.begin(); i != in_stream.end(); ++i) {
        string bin_rep;

        // convert hex to binary
        switch(*i) {
            case '0':
                bin_rep += "0000";
                break;
            case '1':
                bin_rep += "0001";
                break;
            case '2':
                bin_rep += "0010";
                break;
            case '3':
                bin_rep += "0011";
                break;
            case '4':
                bin_rep += "0100";
                break;
            case '5':
                bin_rep += "0101";
                break;
            case '6':
                bin_rep += "0110";
                break;
            case '7':
                bin_rep += "0111";
                break;
            case '8':
                bin_rep += "1000";
                break;
            case '9':
                bin_rep += "1001";
                break;
            case 'A':
                bin_rep += "1010";
                break;
            case 'B':
                bin_rep += "1011";
                break;
            case 'C':
                bin_rep += "1100";
                break;
            case 'D':
                bin_rep += "1101";
                break;
            case 'E':
                bin_rep += "1110";
                break;
            case 'F':
                bin_rep += "1111";
                break;
        }

        bin_stream += bin_rep;
    }

    packet* outer = parse_packet(bin_stream);

    cout << outer->get_version_sum() << endl;
}