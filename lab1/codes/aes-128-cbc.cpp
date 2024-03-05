#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
// aes-128-cbc加密函数
bool aes_128_cbc_encrypt(const uint8_t *in, int in_len, uint8_t *out, int *out_len, const uint8_t *key, const uint8_t *iv)
{
    // 创建上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return false;
    }
    bool ret = false;
    // 初始化加密模块
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) <= 0)
    {
        goto err;
    }
    int update_len;
    // 向缓冲区写入数据，同时将以对齐的数据加密并返回
    if (EVP_EncryptUpdate(ctx, out, &update_len, in, in_len) <= 0)
    {
        goto err;
    }
    int final_len;
    // 结束加密，填充并返回最后的加密数据
    if (EVP_EncryptFinal_ex(ctx, out + update_len, &final_len) <= 0)
    {
        goto err;
    }
    *out_len = update_len + final_len;
    ret = true;
err:
    EVP_CIPHER_CTX_free(ctx);
    return ret;
}
// aes-128-cbc解密函数，结构与加密相似
bool aes_128_cbc_decrypt(const uint8_t *in, int in_len, uint8_t *out, int *out_len, const uint8_t *key, const uint8_t *iv)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return false;
    }
    bool ret = false;
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) <= 0)
    {
        goto err;
    }
    int update_len;
    if (EVP_DecryptUpdate(ctx, out, &update_len, in, in_len) <= 0)
    {
        goto err;
    }
    int final_len;
    if (EVP_DecryptFinal_ex(ctx, out + update_len, &final_len) <= 0)
    {
        goto err;
    }
    *out_len = update_len + final_len;
    ret = true;
err:
    EVP_CIPHER_CTX_free(ctx);
    return ret;
}
int main()
{
    // 密钥
    uint8_t key[] = {35, 31, 71, 44, 34, 42, 76, 16, 86, 27, 93, 59, 26, 62, 4, 19};
    // 初始化向量
    uint8_t iv[] = {91, 66, 51, 17, 14, 40, 65, 38, 4, 60, 89, 44, 87, 63, 67, 32};
    const char *msg = "Hello World!";
    const int msg_len = strlen(msg);
    // 存储密文
    uint8_t ciphertext[32] = {0};
    int ciphertext_len;
    // 加密
    aes_128_cbc_encrypt((uint8_t *)msg, msg_len, ciphertext, &ciphertext_len, (uint8_t *)key, (uint8_t *)iv);
    printf("%s\n", ciphertext);
    // 存储解密后的明文
    uint8_t plaintext[32] = {0};
    int plaintext_len;
    // 解密
    aes_128_cbc_decrypt((uint8_t *)ciphertext, ciphertext_len, plaintext, &plaintext_len, (uint8_t *)key, (uint8_t *)iv);
    // 输出解密后的内容
    printf("%s\n", plaintext);
    return 0;
}