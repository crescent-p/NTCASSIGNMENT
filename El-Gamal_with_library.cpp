#include <iostream>
#include <NTL/ZZ.h>
#include <sstream>
#include <random>

using namespace std;
using namespace NTL;


int randomNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (int)1e9);

    return dis(gen);
}


string ZZToString(const ZZ& number) {
    std::ostringstream oss;
    oss << number;  // Use ostringstream to handle the conversion
    return oss.str();
}



int main(){
    ZZ prime; 
    GenPrime(prime, 512);

    ZZ generator;
    RandomBnd(generator, prime);

    generator = generator%prime;

    ZZ private_key;
    RandomBnd(private_key, prime - 2);

    ZZ public_key = PowerMod(generator, private_key, prime);
    
    string message = "";
    std::cout<<"Type the message to encrypt: ";
    std::getline(std::cin, message);
    //padding to keep the leading zeroes. 072 might get converted to 72 without this
    string asci_string = "111";
    for(int i = 0; i < message.length(); i++){
        string temp = to_string(message[i]);
        while(temp.length() < 3){
            temp = '0' + temp;
        }
        asci_string += temp;
    }
    ZZ m = conv<ZZ>(asci_string.c_str());

    ZZ ephemeral_key;
    RandomBnd(ephemeral_key, prime - 2);

    ZZ c1 = PowerMod(generator, ephemeral_key, prime);
    ZZ c2 = m*PowerMod(public_key, ephemeral_key, prime);
    cout << "Encrypted message (ciphertext): " << c2 << endl;

    ZZ s = PowerMod(c1, private_key, prime);
    ZZ s_inv = InvMod(s, prime);
    ZZ decrypted_message = (c2 * s_inv) % prime;
    
    string ZZstr = ZZToString(decrypted_message);


    string decrypted_decoded = "";
    for(int i = 3; i < ZZstr.length(); i+=3){
        char letter = stoi(ZZstr.substr(i, 3));
        decrypted_decoded += letter;
    }

    cout << "Decrypted message: " << decrypted_decoded << endl;


}