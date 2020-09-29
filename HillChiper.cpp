#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int invKey[2][2];

long mod(long a, long b)
{
	return (a % b + b) % b;
}

void getMatrixInverse(int key[2][2])
{
	int tempKey[2][2];

	tempKey[0][0] = (int)(key[1][1]);
	tempKey[0][1] = (int)((-1) * key[0][1]);
	tempKey[1][0] = (int)((-1) * key[1][0]);
	tempKey[1][1] = (int)(key[0][0]);

	int det = (key[0][0] * key[1][1]) - (key[0][1] * key[1][0]);
	int detInv = 0;
	int flag = 0;

	for (int i = 0; i < 26; i++)
	{
		flag = mod((det * i), 26);
		if (flag == 1)
		{
			detInv = i;
		}
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			if (tempKey[i][j] < 0)
			{
				int tempNum = tempKey[i][j] * detInv;
				invKey[i][j] = ((tempNum % 26) + 26) % 26;
			}
			else
			{
				invKey[i][j] = (tempKey[i][j] * detInv % 26);
			}
		}
}

string encrypt(string plain, int key[2][2])
{
	string cipher = "";
	int strLen = plain.length();

	if (plain.length() % 2 == 1)
		strLen += 1;

	char matrixP[2][strLen];
	int count = 0;

	for (int i = 0; i < strLen / 2; i++)
		for (int j = 0; j < 2; j++)
		{
			if (matrixP[j][i] == 32)
			{
				break;
			}
			matrixP[j][i] = plain[count];
			count++;
		}

	for (int i = 0; i < strLen / 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int tempCipher = 0;
			for (int k = 0; k < 2; k++)
			{
				int l = key[j][k] * (matrixP[k][i] % 65);
				tempCipher += l;
			}
			tempCipher = (tempCipher % 26) + 65;
			cipher += (char)tempCipher;
		}
	}

	return cipher;
}

string decrypt(string cipher, int key[2][2])
{
	string plain = "";
	int strLen = cipher.length();

	if (plain.length() % 2 == 1)
		strLen = cipher.length() + 1;

	getMatrixInverse(key);

	char cipherMatrix[2][strLen / 2];
	int count = 0;

	for (int i = 0; i < strLen / 2; i++)
		for (int j = 0; j < 2; j++)
		{
			cipherMatrix[j][i] = cipher[count];
			count++;
		}

	for (int i = 0; i < cipher.length() / 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int tempPlain = 0;
			for (int k = 0; k < 2; k++)
			{
				int l = invKey[j][k] * (cipherMatrix[k][i] % 65);
				tempPlain += l;
			}
			tempPlain = (tempPlain % 26) + 65;
			plain += (char)tempPlain;
		}
	}

	return plain;
}

int gcd(int a, int b)
{

	if (b > a)
		swap(a, b);

	do
	{
		int temp = a % b;
		a = b;
		b = temp;
	} while (b != 0);

	//Return true if GCD value is equal to one
	return a == 1;
}

int invMod(int m, int n)
{
	int t0 = 0, t1 = 1, invers, q, r, b = m;
	while (r != 1)
	{
		q = m / n;
		r = m % n;
		invers = t0 - q * t1;
		if (invers < 0)
		{
			invers = b - (abs(invers) % b);
		}
		else
		{
			invers %= b;
		}
		t0 = t1;
		t1 = invers;
		m = n;
		n = r;
	}
	return invers;
}

void printMatrix(int matrix[2][2])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void getKey()
{
	string plaintext, ciphertext;
	int key[2][2], det, detInv, adj[2][2], invP[2][2], matrixP[2][2], matrixC[2][2], counter;
	int p, c;

	cout << "\nPlaintext : ";
	cin.ignore();
	getline(cin, plaintext);

	//transform plaintext into matrix
	counter = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			p = toupper(plaintext[counter]) - 65;
			matrixP[i][j] = p;
			counter++;
		}
	}

	cout
		<< "\nCiphertext : ";
	getline(cin, ciphertext);

	//transform ciphertext into matrix
	counter = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			c = toupper(ciphertext[counter]) - 65;
			matrixC[i][j] = c;
			counter++;
		}
	}

	det = (matrixP[0][0] * matrixP[1][1]) - (matrixP[0][1] * matrixP[1][0]);

	det = mod(det, 26);

	//check wether or not the matrixP has its inverse value by checking the GCD value
	if (gcd(26, det))
	{
		detInv = invMod(26, det);

		//calculate the adjoint matrix
		adj[0][0] = matrixP[1][1];
		adj[0][1] = (-1) * matrixP[0][1];
		adj[1][0] = (-1) * matrixP[1][0];
		adj[1][1] = matrixP[0][0];

		//caltulate the invers matrix of plaintext
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				invP[i][j] = detInv * adj[i][j];
				if (invP[i][j] < 0)
				{
					invP[i][j] = 26 - (abs(invP[i][j]) % 26);
				}
				else
				{
					invP[i][j] = invP[i][j];
					invP[i][j] = invP[i][j] % 26;
				}
			}
		}

		//generate the key
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				key[i][j] = 0;
				for (int k = 0; k < 2; k++)
				{
					key[i][j] += (invP[i][k] * matrixC[k][j]);
				}
				key[i][j] %= 26;
			}
		}

		cout << "\nKey : " << endl;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				cout << key[j][i] << " ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Error! Unable to find the key!" << endl;
		cout << "Matrix P has no inverse" << endl
			 << endl;
	}
}

int main()
{
	int key[2][2];

	string plain, cipher;
	int menu;

	cout << "HILLCIPHER \n\nMenu : \n1.Encrypt \n2.Decrypt \n3.Find Key \nChoose Menu :";
	cin >> menu;

	switch (menu)
	{
	case 1:
		cout << "\nPlaintext: ";
		cin.ignore();
		getline(cin, plain);
		transform(plain.begin(), plain.end(), plain.begin(), ::toupper);

		cout << "\nKey (2x2 matrix) : ";
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
			{
				cin >> key[i][j];
			}

		cout << "\nCiphertext : " << encrypt(plain, key) << endl
			 << endl;
		break;
	case 2:
		cout << "\nCiphertext: ";
		cin.ignore();
		getline(cin, cipher);
		transform(cipher.begin(), cipher.end(), cipher.begin(), ::toupper);

		cout << "\nKey (2x2 matrix) : ";
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
			{
				cin >> key[i][j];
			}

		cout << "\nPlaintext : " << decrypt(cipher, key) << endl
			 << endl;
		break;
	case 3:
		getKey();
		break;
	default:
		cout << "\nInvalid!" << endl;
		break;
	}

	return 0;
}
