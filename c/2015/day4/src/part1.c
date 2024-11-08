#include <aoc.h>
#include <assert.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>

#define MAX_KEY 100

EVP_MD_CTX *ctx = NULL;
EVP_MD *md5 = NULL;

int md5_hash(char *value) {
  assert(ctx != NULL);
  assert(md5 != NULL);

  unsigned char outdigest[EVP_MAX_MD_SIZE];
  unsigned int len;
  char comp[3] = {0};
  int index = 0;
  int found_char = 0;

  EVP_DigestInit_ex(ctx, md5, NULL);

  EVP_DigestUpdate(ctx, value, strlen(value));

  EVP_DigestFinal(ctx, outdigest, &len);

  for (unsigned int i = 0; i < len; ++i) {
    snprintf(comp, 3, "%02X", outdigest[i]);
    printf("%02X", outdigest[i]);

    for (int j = 0; j < 2; ++j) {
      if (comp[j] == '0' && !found_char) {
        ++index;
      } else {
        found_char = 1;
      }
    }
  }
  putchar('\n');

  return index;
}

int main(void) {
  size_t counter = 0;
  char secret_key[MAX_KEY] = {0};
  char digest_value[MAX_KEY] = {0};

  ctx = EVP_MD_CTX_new();
  md5 = EVP_MD_fetch(NULL, "MD5", NULL);
  if (aoc_get_line(secret_key, MAX_KEY) == 0) {
    aoc_fatalf("error reading line from stdin");
  }

  printf("Secret Key: %s\n", secret_key);

  while (1) {
    snprintf(digest_value, MAX_KEY, "%s%zu", secret_key, counter);
    printf("Digest value: %s\n", digest_value);

    int zero_count = md5_hash(digest_value);

    printf("%d\n", zero_count);

    if (zero_count == 5) {
      break;
    }

    counter++;
  }

  printf("Answer: %zu\n", counter);

  EVP_MD_free(md5);
  EVP_MD_CTX_free(ctx);

  return 0;
}
