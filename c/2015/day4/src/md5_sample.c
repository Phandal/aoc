#include <aoc.h>
#include <openssl/evp.h>
#include <stdio.h>

int main(void) {
  EVP_MD_CTX *ctx = NULL;
  EVP_MD *md5 = NULL;
  unsigned char outdigest[EVP_MAX_MD_SIZE];
  unsigned int len;

  // Create a context for the digest operation
  ctx = EVP_MD_CTX_new();
  if (ctx == NULL) {
    aoc_fatalf("failed to init evp ctx");
  }

  md5 = EVP_MD_fetch(NULL, "MD5", NULL);
  if (md5 == NULL) {
    EVP_MD_free(md5);
    EVP_MD_CTX_free(ctx);
    aoc_fatalf("failed to fetch digest");
  }

  if (!EVP_DigestInit_ex(ctx, md5, NULL)) {
    EVP_MD_free(md5);
    EVP_MD_CTX_free(ctx);
    aoc_fatalf("failed to init digest");
  }

  EVP_DigestUpdate(ctx, "Hello", 5);

  EVP_DigestFinal_ex(ctx, outdigest, &len);

  for (unsigned int i = 0; i < len; ++i) {
    printf("%02x", outdigest[i]);
  }
  putchar('\n');

  return 0;
}
