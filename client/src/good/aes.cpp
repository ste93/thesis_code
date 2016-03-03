#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "const_cli.h"

#include "aes.h"
unsigned char key[32];
//unsigned char iv[16];

void handleErrors(void)
{
    unsigned long errCode;

    printf("An error occurred\n");
    while(errCode = ERR_get_error())
    {
        char *err = ERR_error_string(errCode, NULL);
        printf("%s\n", err);
    }
    abort();
}



int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}




int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}





int initaes ()
{
  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */
    ERR_load_crypto_strings();  
	//CryptoPP::AutoSeededRandomPool rnd;	
	
  /* A 256 bit key */
  //unsigned char key[32];


  /* A 128 bit IV */
  //unsigned char iv[16];
	std::ifstream fkey;
	std::ofstream fkeyof;
	fkey.open("key.aes", std::ios_base::binary);
    if (fkey) {
		fkey.read((char *)key, 32);
		fkey.close();
    } else {
		fkey.close();
		fkeyof.open("key.aes", std::ios_base::binary);
		if (!RAND_bytes((unsigned char *)key, sizeof key)) {
			return 1;
		}
		fkeyof.write((const char *)key, 32);
		fkeyof.close();
	}   
	
/*	std::ifstream fiv;
	std::ofstream fivof;
	fiv.open("iv.aes", std::ios_base::binary);
    if (fiv) {
		fiv.read((char *)iv, 16);
		fiv.close();
    } else {
		fiv.close();
		if (!RAND_bytes((unsigned char *)iv, sizeof iv)) {
			return 1; 
		}
		fivof.open("iv.aes", std::ios_base::binary);
		fivof.write((const char *)iv, 16);
		fivof.close();
    }   

*/


  /* Message to be encrypted */
 // unsigned char *plaintext =            (unsigned char *)"The quick brown fox jumps over the lazy dog";

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */

  //

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);
  return 0;
}
  /* Encrypt the plaintext */
  //

  /* Do something useful with the ciphertext here */
  //printf("Ciphertext is:\n");
  //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */
  //
	/*
	

  /* Add a NULL terminator. We are expecting printable text */
 // decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  //printf("Decrypted text is:\n");
  //printf("%s\n", decryptedtext);

  /* Clean up */
  //EVP_cleanup();
  //ERR_free_strings();

  //return 0;
//}


/*int main() {
	initaes();
	int length = sizeof(node);
	node *n = new node;
	n->num_keys= 22;
	int tot_len = length + 10 + ((length+10) % 16);
	std::cout << tot_len;
	unsigned char * str = (unsigned char *)malloc(tot_len);
	memcpy(str, n, length);
	if (!RAND_bytes(&str[sizeof(node)], 10 + ((sizeof(node)+10) % 16))) {
		return 1;
	}
	std::cout << tot_len;
	int decryptedtext_len, ciphertext_len;
	unsigned char ciphertext[512];
	unsigned char decryptedtext[512];
	ciphertext_len = encrypt (str,tot_len, key, iv,ciphertext);
	decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
node * mm;
	mm =(node *)decryptedtext;
	std::cout << mm->num_keys;
	return 0;
}*/