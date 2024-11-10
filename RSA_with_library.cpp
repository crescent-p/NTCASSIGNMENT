#include <iostream>
#include <NTL/ZZ.h>
#include <sstream>

using namespace std;
using namespace NTL;


ZZ generate_prime(int bits) {
    ZZ p;
    GenPrime(p, bits);
    return p;
}

ZZ mod_inverse(const ZZ& a, const ZZ& m) {
    ZZ x;
    InvMod(x, a, m); 
    return x;
}

void generate_rsa_keys(ZZ& n, ZZ& e, ZZ& d, int bits) {
    ZZ p = generate_prime(bits / 2); 
    ZZ q = generate_prime(bits / 2); 
    n = p * q;                       
    ZZ phi_n = (p - 1) * (q - 1);   

    e = generate_prime(500);                   
    d = mod_inverse(e, phi_n);       
}


ZZ encrypt(const ZZ& m, const ZZ& e, const ZZ& n) {
    return PowerMod(m, e, n); 
}


ZZ decrypt(const ZZ& c, const ZZ& d, const ZZ& n) {
    return PowerMod(c, d, n); 
}
string ZZToString(const ZZ& number) {
    std::ostringstream oss;
    oss << number;  // Use ostringstream to handle the conversion
    return oss.str();
}

int main() {
    ZZ n, e, d;
    int bits = 512;
    generate_rsa_keys(n, e, d, bits);

    cout << "Public Key (n, e):" << endl;
    cout << "n = " << n << endl;
    cout << "e = " << e << endl;

    cout << "Private Key (d):" << endl;
    cout << "d = " << d << endl;


    string message = "";
    std::cout<<"Type the message to encrypt: ";
    std::cin>>message;
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

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted message (ciphertext): " << c << endl;

    ZZ decrypted_message = decrypt(c, d, n);
    
    string ZZstr = ZZToString(decrypted_message);


    string decrypted_decoded = "";
    for(int i = 3; i < ZZstr.length(); i+=3){
        char letter = stoi(ZZstr.substr(i, 3));
        decrypted_decoded += letter;
    }


    cout << "Decrypted message: " << decrypted_decoded << endl;

    return 0;
}
