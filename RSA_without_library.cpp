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

ZZ mypow(ZZ a, ZZ b){
    ZZ result = ZZ(1);
    ZZ multi = ZZ(a);
    while(b > 0){
        if(b % 2 == 1){
            result *= multi;
        }
        multi *= multi;
        b >>= 1;
    }
    return result;
}


ZZ generate_large_number(int bits){
    ZZ value = ZZ(1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for(int i = 0; i < bits; i++){
        if(dis(gen) == 0){
            value += mypow(ZZ(2), ZZ(i));
        }
    }
    
    return value;
}

ZZ myPowModN(ZZ a, ZZ b, ZZ mod){
    ZZ result = ZZ(1);
    ZZ multi = ZZ(a);
    while(b > 0){
        if(b%2 == 1){
            result *= multi;
            result %= mod;
        }
        multi *= multi;
        multi %= mod;
        b = b>>1;
    }
    return result%mod;
}


bool myProbPrime(ZZ number){
    if(number < 2 || number %2 == 0) return false;

    ZZ d = number - 1;
    ZZ s = ZZ(0);
    while(d % 2 == 0){
        s++;
        d /= 2;
    }

    ZZ random_between_two_and_n_minus_2 = number/randomNumber();

    ZZ x_a_power_d_mod_n = myPowModN(random_between_two_and_n_minus_2, d, number);

    if(x_a_power_d_mod_n == 1 || x_a_power_d_mod_n == (number - 1)){
        return true;
    }
    int tries = 2000;

    while(tries > 0){
        if(x_a_power_d_mod_n == 1 || x_a_power_d_mod_n == (number - 1)){
            return true;
        }
        x_a_power_d_mod_n = myPowModN(x_a_power_d_mod_n, ZZ(2), number);
        tries--;
    }
    return false;
}


ZZ generate_prime(int bits){
    ZZ prime;
    // generate large number
    int tries = 0;
    ZZ number = generate_large_number(bits);
    while(!myProbPrime(number)){
        number = generate_large_number(bits);
        cout<<number<<endl;
        tries++;
    }
    cout<<"Number of tries: "<<tries<<endl;

    return number;

}

ZZ extended_gcd(ZZ a, ZZ b, ZZ& x, ZZ& y){
    if(a == 0){
        x = 0;
        y = 1;
        return b;
    }

    ZZ x1, y1;
    ZZ gcd = extended_gcd(b%a, a, x1, y1);

    x = y1 - (b/a) * x1;
    y = x1;

    return gcd;
}



ZZ modInv(ZZ e, ZZ d){
    // e * d = 1 (d)
    ZZ x, y;
    ZZ gcd = extended_gcd(e, d, x, y);

    if(gcd != 1){
        return ZZ(-1);
    }else{
        return x%d;
    }
}

int main(){
    ZZ p = generate_prime(512/2);
    ZZ q = generate_prime(512/2);

    ZZ n = p * q;

    ZZ eulers_totient = (p - 1) * (q - 1);

    ZZ e = generate_prime(512 - 2);

    ZZ d = modInv(e, eulers_totient);

    cout<<"p: "<<p<<endl;
    cout<<"q: "<<q<<endl;
    cout<<"n: "<<n<<endl;
    cout<<"e: "<<e<<endl;
    cout<<"d: "<<d<<endl;
    cout<<"e*d (totient): "<< (e*d) %eulers_totient<<endl;

    cout<<"Message to encrypt: ";
    
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

    ZZ c = myPowModN(m, e, n);
    cout << "Encrypted message (ciphertext): " << c << endl;

    ZZ decrypted_message = myPowModN(c, d, n);
    
    string ZZstr = ZZToString(decrypted_message);


    string decrypted_decoded = "";
    for(int i = 3; i < ZZstr.length(); i+=3){
        char letter = stoi(ZZstr.substr(i, 3));
        decrypted_decoded += letter;
    }


    cout << "Decrypted message: " << decrypted_decoded << endl;

    return 0;
}