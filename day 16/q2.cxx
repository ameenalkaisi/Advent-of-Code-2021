#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <bitset>
#include <bit>
#include <numeric>

using namespace std;

// in this one I defined arithmetic operations for binary numbers in bitsets
// then did the same thing as q1, but at the end i displayed ullong using bitset's to_ullong method
//
// this could be simplified further by creating a class (e.g., big_binary) that stores big binary nums and it would
// make the code easier to look at and all its operations contained within

constexpr int max_bits = 1000;

bitset<max_bits> operator+(bitset<max_bits> const &a, bitset<max_bits> const &b) {
    bitset<max_bits> c;

    // goes through all elements and adds the numbers accordingly
    for(int i = 0; i < max_bits; ++i) {
        if(c[i] + a[i] + b[i] == 3) {
            c[i + 1] = 1;
            c[i] = 1; 
        } else if(c[i] + a[i] + b[i] == 2) {
            c[i + 1] = 1;
            c[i] = 0;
        } else if(c[i] + a[i] + b[i] == 1)
            c[i] = 1;
    }

    return c;
}

// define multiplication
bitset<max_bits> operator*(bitset<max_bits> const &a, bitset<max_bits> const &b) {
    bitset<max_bits> c;

    for(int i = 0; i < max_bits; ++i) {
        // for the new row in multiplication
        bitset<max_bits> middle;

        for(int j = 0; j < max_bits; ++j)
            middle[j] = b[i]*a[j];

        // shift it
        middle <<= i;

        // add it to current total
        c = c + middle;
    }
    
    return c;
}

bool operator<(bitset<max_bits> const &a, bitset<max_bits> const &b) {
    for(int i = max_bits - 1; i >= 0; --i) {
        if(a[i] == b[i])
            continue;

        return a[i] < b[i];
    }

    return false;
}

int operator>(bitset<max_bits> const &a, bitset<max_bits> const &b) {
    return b < a;
}

struct header {
    int version, type_id;

    header(int version, int type_id) : version{version}, type_id{type_id} {}
};

struct packet {
    header packet_header;

    packet(header packet_header) : packet_header{packet_header} {}

    virtual unsigned int get_version_sum() {
        return -1;
    };

    virtual bitset<max_bits> value() {
        return {};
    };
};

struct literal_value_packet : public packet {
    bitset<max_bits> number;

    literal_value_packet(header packet_header, bitset<max_bits> number) : packet::packet(packet_header), number{number} {}

    unsigned int get_version_sum() override {
        return packet_header.version;
    }
    
    bitset<max_bits> value() override {
        return number;
    }
};

struct operator_packet : public packet {
    vector<packet*> sub_packets;
    int length_type_id;
    unsigned int result;

    // intialize sub packets later
    operator_packet(header packet_header, int length_type_id, vector<packet*> sub_packets) : packet::packet(packet_header), length_type_id{length_type_id},
        sub_packets{sub_packets} {}

    bool is_total_length() const {
        return length_type_id == 0;
    }

    bool is_sub_packet_count() const {
        return length_type_id == 1;
    }

    unsigned int get_version_sum() override {
        int total = packet_header.version; 
        for(auto i : sub_packets)
            total += i->get_version_sum();

        return total;
    }

    bitset<max_bits> value() override {
        bitset<max_bits> total{0};

        // value depends on type id
        switch(packet_header.type_id) {
            case 0:
                total = accumulate(sub_packets.begin(), sub_packets.end(), bitset<max_bits>{0}, [](auto const &a, auto const &b) {
                    return a + b->value();
                });
                break;
            case 1:
                total = accumulate(sub_packets.begin(), sub_packets.end(), bitset<max_bits>{1}, [](auto const &a, auto const &b) {
                    return a*b->value();
                });
                break;
            case 2:
                // starts with first element's value
                total = accumulate(sub_packets.begin() + 1, sub_packets.end(), sub_packets[0]->value(), [](auto const &a, auto const &b) {
                    auto b_val = b->value();
                    return a < b_val ? a : b_val;
                });
                break;
            case 3:
                // starts with first element's value
                // starts with first element's value
                total = accumulate(sub_packets.begin() + 1, sub_packets.end(), sub_packets[0]->value(), [](auto const &a, auto const &b) {
                    auto b_val = b->value();
                    return a > b_val ? a : b_val;
                });
                break;
            case 5:
                total = sub_packets[1]->value() < sub_packets[0]->value(); // greater than
                break;
            case 6:
                total = sub_packets[0]->value() < sub_packets[1]->value(); // less than
                break;
            case 7:
                total = sub_packets[0]->value() == sub_packets[1]->value(); // equal
                break;
        }

        return total;
    }
};

packet* parse_packet_(string const &bin_stream, int &index) {
    // get version and type id
    int version = static_cast<int>(bitset<3>{bin_stream.substr(index, 3)}.to_ullong());
    index += 3;

    int type_id = static_cast<int>(bitset<3>{bin_stream.substr(index, 3)}.to_ullong());
    index += 3;

    header packet_header{version, type_id};

    if(type_id == 4) {
        // parse the number
        bitset<max_bits> num;
        while(true) {
            bool stop = bin_stream[index++] == '0';

            // get the 4 digits from this one
            for(int i = 0; i < 4; ++i) {
                num <<= 1;
                num[0] = bin_stream[index++] - '0';
            }

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
    if(!input && argc == 2)
        exit(1);


    string in_stream;
    getline(input, in_stream);

    if(argc == 3)
        in_stream = argv[2];
        
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
    
    // note, converting to ullong at the end makes it work
    // but needed binary calculations in between
    cout << outer->value().to_ullong() << endl; 
}