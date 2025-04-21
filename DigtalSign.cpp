#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int EXCLUDED_PUBLIC_EXPONENTS[] = {2, 3, 5, 7, 11};

int compute_gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool is_excluded(int value) {
    for (int exp : EXCLUDED_PUBLIC_EXPONENTS) {
        if (value == exp) return true;
    }
    return false;
}

bool check_prime(int number) {
    if (number < 2) return false;
    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) return false;
    }
    return true;
}

long long modular_exponentiation(long long base, long long exponent, long long modulus) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exponent /= 2;
    }
    return result;
}

int find_mod_inverse(int public_exp, int phi_n) {
    for (int private_exp = 12; private_exp < phi_n; private_exp++) {
        if ((public_exp * private_exp) % phi_n == 1) return private_exp;
    }
    return -1;
}

void generate_rsa_keypair(int &prime1, int &prime2, int &modulus, int &public_exp, int &private_exp) {
    if (!check_prime(prime1) || !check_prime(prime2) || prime1 == prime2) {
        cout << "Invalid input! Prime numbers must be distinct and valid." << endl;
        exit(EXIT_FAILURE);
    }

    modulus = prime1 * prime2;
    int phi_n = (prime1 - 1) * (prime2 - 1);

    srand(time(0));
    do {
        public_exp = 12 + rand() % (phi_n - 12);
    } while (compute_gcd(public_exp, phi_n) != 1 || is_excluded(public_exp));

    private_exp = find_mod_inverse(public_exp, phi_n);
    if (private_exp == -1) {
        cout << "Error: No modular inverse found for the public exponent. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    int prime1, prime2, modulus, public_key_exp, private_key_exp;

    cout << "Enter a prime number (p): ";
    cin >> prime1;
    cout << "Enter another prime number (q): ";
    cin >> prime2;

    generate_rsa_keypair(prime1, prime2, modulus, public_key_exp, private_key_exp);
    cout << "\nPublic Key (n, e): (" << modulus << ", " << public_key_exp << ")";
    cout << "\nPrivate Key (n, d): (" << modulus << ", " << private_key_exp << ")\n";

    int plain_message;
    cout << "\nEnter a number to send (must be less than " << modulus << "): ";
    cin >> plain_message;

    if (plain_message >= modulus) {
        cout << "Message must be less than " << modulus << ". Exiting.\n";
        return 1;
    }

    int encrypted_msg = modular_exponentiation(plain_message, public_key_exp, modulus);
    cout << "\nEncrypted Message: " << encrypted_msg;

    int digital_signature = modular_exponentiation(plain_message, private_key_exp, modulus);
    cout << "\nDigital Signature: " << digital_signature;

    cout << "\n\nReceiver Side:";

    int decrypted_msg = modular_exponentiation(encrypted_msg, private_key_exp, modulus);
    cout << "\nDecrypted Message: " << decrypted_msg;

    int verified_signature = modular_exponentiation(digital_signature, public_key_exp, modulus);
    cout << "\nVerified Signature: " << verified_signature;

    if (verified_signature == decrypted_msg) {
        cout << "\n\nSignature is VALID. Message is authentic and untampered.\n";
    } else {
        cout << "\n\nSignature is INVALID. Message may have been tampered with.\n";
    }

    return 0;
}
