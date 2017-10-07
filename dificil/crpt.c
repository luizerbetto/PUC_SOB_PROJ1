struct tcrypt_result {
    struct completion completion;
    int err;
};

//asm-generic/scatterlist
//
/* tie all data structures together */
struct skcipher_def {
    struct scatterlist sg;
    struct crypto_skcipher *tfm; //controlador de cifra, nescessario para invocação API da chave simetrica
    struct skcipher_request *req; // requisição de chave simetrica de cifra
    struct tcrypt_result result;
};


static void test_skcipher_cb(struct crypto_async_request *req, int error)
{
    struct tcrypt_result *result = req->data;

    if (error == -EINPROGRESS)
        return;
    result->err = error;
    complete(&result->completion);
    pr_info("Encryption finished successfully\n");
}

/* Callback function */


/* Perform cipher operation */
static unsigned int test_skcipher_encdec(struct skcipher_def *sk,
                     int enc)
{
    int rc = 0;

    if (enc)
        rc = crypto_skcipher_encrypt(sk->req); // criptografa plaintext
    else
        rc = crypto_skcipher_decrypt(sk->req); // descriptografa texto

    switch (rc) {
    case 0:
        break;
    case -EINPROGRESS:
    case -EBUSY:
        rc = wait_for_completion_interruptible( /*espera o final de uma tarefa assinalada*/
            &sk->result.completion);
        if (!rc && !sk->result.err) {
            reinit_completion(&sk->result.completion);
            break;
        }
    default:
        pr_info("skcipher encrypt returned with %d result %d\n",
            rc, sk->result.err);
        break;
    }
    init_completion(&sk->result.completion);

    return rc;
}

/* Inicialização e ativação operação de cifra*/
static int test_skcipher(void)
{
    struct skcipher_def sk; // estrutura principal contendo todos dados nescessarios
    struct crypto_skcipher *skcipher = NULL; //controlador de cifra, nescessario para invocação API da chave simetrica
    struct skcipher_request *req = NULL; // requisição de chave simetrica de cifra
    char *scratchpad = NULL;
    char *ivdata = NULL; //vetor inicialização -- aleatoriza os caracteres de criptografia, manter aleatorio
    unsigned char key[32]; // chave
    int ret = -EFAULT;

    skcipher = crypto_alloc_skcipher("cbc-aes-aesni", 0, 0);/* aloca o controlador da chave simetrica de sifra
    *qual tipo de algoritimo: troca para ecb-aes
    *tipo de cifra
    *mascara da cifra
    */
    if (IS_ERR(skcipher)) {
        pr_info("could not allocate skcipher handle\n");
        return PTR_ERR(skcipher);
    }

    req = skcipher_request_alloc(skcipher, GFP_KERNEL);/*aloca uma estrutura de dados para ser usada com o skcipher
    *controlador de cifra que será registrado com o pedido
    *bandeira de alocação de memoria
    */
    if (!req) {
        pr_info("could not allocate skcipher request\n");
        ret = -ENOMEM;
        goto out;
    }

    skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG, test_skcipher_cb, &sk.result); 
    /*função chamada quando completar o processo
    *manipulador da estrutura
    *bandeira de sinalização 
    *chamada de função a ser registrada com a estrutura
    *estrutura onde será guardado o resultado da operação de cifra
    

    /* AES 256 with random key */
    get_random_bytes(&key, 32);
    if (crypto_skcipher_setkey(skcipher, key, 32)) {
        pr_info("key could not be set\n");
        ret = -EAGAIN;
        goto out;
    }

    /* IV will be random */
    ivdata = kmalloc(16, GFP_KERNEL);
    if (!ivdata) {
        pr_info("could not allocate ivdata\n");
        goto out;
    }
    get_random_bytes(ivdata, 16);

    /* Input data will be random */
    scratchpad = kmalloc(16, GFP_KERNEL);
    if (!scratchpad) {
        pr_info("could not allocate scratchpad\n");
        goto out;
    }
    get_random_bytes(scratchpad, 16);

    sk.tfm = skcipher;
    sk.req = req;

    /* We encrypt one block */
    sg_init_one(&sk.sg, scratchpad, 16); /*
    *inciando uma lista SG,
    *passamos a lista, endereço de entrada e saida, comprimento do endereço
    */
    skcipher_request_set_crypt(req, &sk.sg, &sk.sg, 16, ivdata); // configurando o destino e a origem da lista SG
    init_completion(&sk.result.completion);

    /* encrypt data */
    ret = test_skcipher_encdec(&sk, 1); //função propria do codigo
    if (ret)
        goto out;

    pr_info("Encryption triggered successfully\n");

out:
    if (skcipher)
        crypto_free_skcipher(skcipher); //libera o controlador
    if (req)
        skcipher_request_free(req); // libera a estrutura
    if (ivdata)
        kfree(ivdata); // libera o iv
    if (scratchpad)
        kfree(scratchpad); // libera a data de entrada
    return ret;
}
