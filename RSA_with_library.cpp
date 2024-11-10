#include <iostream>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

using ll = NTL::ZZ;


ll generate_prime(int bits) {
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

    e = ZZ(65537);
    ZZ k = conv<ZZ>("");
    double val = ProbPrime(k);
    std::cout<<"prob"<<val<<endl;                   
    d = mod_inverse(e, phi_n);       
}


ZZ encrypt(const ZZ& m, const ZZ& e, const ZZ& n) {
    return PowerMod(m, e, n); 
}


ZZ decrypt(const ZZ& c, const ZZ& d, const ZZ& n) {
    return PowerMod(c, d, n); 
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
    cout<<"Type the message to encrypt: ";
    cin>>message;
    string asci_string = "";
    for(int i = 0; i < message.length(); i++){
        asci_string += to_string(message[i]);
    }
    ZZ m = conv<ZZ>(asci_string);

    ZZ c = encrypt(m, e, n);
    cout << "Encrypted message (ciphertext): " << c << endl;

    ZZ decrypted_message = decrypt(c, d, n);
    cout << "Decrypted message: " << decrypted_message << endl;

    return 0;
}
