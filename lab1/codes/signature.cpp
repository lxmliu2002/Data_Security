#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
// 公钥文件名
#define PUBLIC_KEY_FILE_NAME "public.pem"
// 私钥文件名
#define PRIVATE_KEY_FILE_NAME "private.pem"
// RSA生成公私钥，存储到文件
bool genrsa(int numbit)
{
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx)
    {
        return false;
    }
    EVP_PKEY *pkey = NULL;
    bool ret = false;
    int rt;
    FILE *prif = NULL, *pubf = NULL;
    if (EVP_PKEY_keygen_init(ctx) <= 0)
    {
        goto err;
    }
    // 设置密钥长度
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, numbit) <= 0)
    {
        goto err;
    }
    // 生成密钥
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
    {
        goto err;
    }
    prif = fopen(PRIVATE_KEY_FILE_NAME, "w");
    if (!prif)
    {
        goto err;
    }
    // 输出私钥到文件
    rt = PEM_write_PrivateKey(prif, pkey, NULL, NULL, 0, NULL, NULL);
    fclose(prif);
    if (rt <= 0)
    {
        goto err;
    }
    pubf = fopen(PUBLIC_KEY_FILE_NAME, "w");
    if (!pubf)
    {
        goto err;
    }
    // 输出公钥到文件
    rt = PEM_write_PUBKEY(pubf, pkey);
    fclose(pubf);
    if (rt <= 0)
    {
        goto err;
    }
    ret = true;
err:
    EVP_PKEY_CTX_free(ctx);
    return ret;
}
// 生成数据签名
bool gensign(const uint8_t *in, unsigned int in_len, uint8_t *out, unsigned int *out_len)
{
    FILE *prif = fopen(PRIVATE_KEY_FILE_NAME, "r");
    if (!prif)
    {
        return false;
    }
    // 读取私钥
    EVP_PKEY *pkey = PEM_read_PrivateKey(prif, NULL, NULL, NULL);
    fclose(prif);
    if (!pkey)
    {
        return false;
    }
    bool ret = false;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
    {
        goto ctx_new_err;
    }
    // 初始化
    if (EVP_SignInit(ctx, EVP_sha256()) <= 0)
    {
        goto sign_err;
    }
    // 输入消息，计算摘要
    if (EVP_SignUpdate(ctx, in, in_len) <= 0)
    {
        goto sign_err;
    }
    // 生成签名
    if (EVP_SignFinal(ctx, out, out_len, pkey) <= 0)
    {
        goto sign_err;
    }
    ret = true;
sign_err:
    EVP_MD_CTX_free(ctx);
ctx_new_err:
    EVP_PKEY_free(pkey);
    return ret;
}
// 使用公钥验证数字签名，结构与签名相似
bool verify(const uint8_t *msg, unsigned int msg_len, const uint8_t *sign, unsigned int sign_len)
{
    FILE *pubf = fopen(PUBLIC_KEY_FILE_NAME, "r");
    if (!pubf)
    {
        return false;
    }
    // 读取公钥
    EVP_PKEY *pkey = PEM_read_PUBKEY(pubf, NULL, NULL, NULL);
    fclose(pubf);
    if (!pkey)
    {
        return false;
    }
    bool ret = false;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
    {
        goto ctx_new_err;
    }
    // 初始化
    if (EVP_VerifyInit(ctx, EVP_sha256()) <= 0)
    {
        goto sign_err;
    }
    // 输入消息，计算摘要
    if (EVP_VerifyUpdate(ctx, msg, msg_len) <= 0)
    {
        goto sign_err;
    }
    // 验证签名
    if (EVP_VerifyFinal(ctx, sign, sign_len, pkey) <= 0)
    {
        goto sign_err;
    }
    ret = true;
sign_err:
    EVP_MD_CTX_free(ctx);
ctx_new_err:
    EVP_PKEY_free(pkey);
    return ret;
}
int main()
{
    // 生成长度为2048的密钥
    genrsa(2048);
    const char *msg = "Hello World!";
    const unsigned int msg_len = strlen(msg);
    // 存储签名
    uint8_t sign[256] = {0};
    unsigned int sign_len = 0;
    // 签名
    if (!gensign((uint8_t *)msg, msg_len, sign, &sign_len))
    {
        printf("签名失败\n");
        return 0;
    }
    // 验证签名
    if (verify((uint8_t *)msg, msg_len, sign, sign_len))
    {
        printf("验证成功\n");
    }
    else
    {
        printf("验证失败\n");
    }
    return 0;
}