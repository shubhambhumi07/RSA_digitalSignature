#include <iostream>
#include <cmath>
using namespace std;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int mod_inverse(int e, int phi) {
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1;
}
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return false;
    }
    return true;
}
void generate_rsa_keys(int &p, int &q, int &n, int &e, int &d) {
    if (!is_prime(p) || !is_prime(q) || p == q) {
        cout << "Invalid input! p and q must be distinct prime numbers." << endl;
        exit(EXIT_FAILURE);
    }

    n = p * q;
    int phi = (p - 1) * (q - 1);

    e = 2;
    while (e < phi && gcd(e, phi) != 1) {
        e++;
    }
    d = mod_inverse(e, phi);
    if (d == -1) {
        cout << "Error: Could not find modular inverse for 'e'. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    int p, q, n, e, d;

    cout << "Enter a prime number (p): ";
    cin >> p;
    cout << "Enter another prime number (q): ";
    cin >> q;

    generate_rsa_keys(p, q, n, e, d);

    cout << "Public Key (n, e): (" << n << ", " << e << ")" << endl;
    cout << "Private Key (n, d): (" << n << ", " << d << ")\n" << endl;

    int message;
    cout << "Enter a number to encrypt (must be less than " << n << "): ";
    cin >> message;

    int encrypted = mod_pow(message, e, n);
    cout << "Encrypted Message: " << encrypted << endl;

    int decrypted = mod_pow(encrypted, d, n);
    cout << "Decrypted Message: " << decrypted << endl;

    return 0;
}
