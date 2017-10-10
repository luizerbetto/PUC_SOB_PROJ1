/*
*comando para dar permissão ao user.c usar esse device
*sudo chmod 666 /dev/crypto 
*/


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 
#include <linux/device.h>
#include <linux/completion.h>
#include <linux/scatterlist.h>
#include <crypto/skcipher.h>
#include <linux/random.h>

#define DEVICE_NAME "crypto"   /* Dev name as it appears in /proc/devices   */
#define CLASS_NAME "crypto"
#define BUF_LEN 80              /* Max length of the message from the device */

#ifndef AF_ALG
#define AF_ALG 38
#endif

// Prototypes - this would normally go in a .h file
//static int cryptomodule_init(void);
//static void cryptomodule_exit(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static char *key = "12345678910111213";
static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open? Used to prevent multiple access to device */

static char msg[BUF_LEN];       /* The msg the device will give when asked */
static char *msg_Ptr;

static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored

static struct class*  criptoClass  = NULL; ///< The device-driver class struct pointer
static struct device* criptoDevice = NULL; ///< The device-driver device struct pointer


//respostas através do arquivo de dispositivo /dev/crypto
//c-cifrar(write) d-decrifrar(read) h-resumo criptografico(?)
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};


module_param(key, charp, 0000);
MODULE_PARM_DESC(key, "key = ");


//---------------------Área de API do codigo----------------------------------------//

struct tcrypt_result {
    struct completion completion;
    int err;
};

/* tie all data structures together */
struct skcipher_def {
    struct scatterlist sg;
    struct crypto_skcipher *tfm; //controlador de cifra, nescessario para invocação API da chave simetrica
    struct skcipher_request *req; // requisição de chave simetrica de cifra
    struct tcrypt_result result;
};



/* Callback function */
static void test_skcipher_cb(struct crypto_async_request *req, int error)
{
    struct tcrypt_result *result = req->data;

    if (error == -EINPROGRESS)
        return;
    result->err = error;
    complete(&result->completion);
    pr_info("Encryption finished successfully\n");
};


/* Perform cipher operation */
static unsigned int test_skcipher_encdec(struct skcipher_def *sk,
                     char enc)
{
    int rc = 0;

    if (enc == 'c')
	{
		rc = crypto_skcipher_encrypt(sk->req); // criptografa plaintext
		
	}
    else{
	if(enc == 'd')
		{
			rc = crypto_skcipher_decrypt(sk->req); // descriptografa texto
			//sprintf(message,"descriptografia");
		}
	else
	{
		sprintf(message,"invalido");
	}
    }


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
    int ret = -EFAULT;
    char *buf;

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
    pr_info(KERN_INFO "my req is read");
    skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG, test_skcipher_cb, &sk.result); 
    /*função chamada quando completar o processo
    *manipulador da estrutura
    *bandeira de sinalização 
    *chamada de função a ser registrada com a estrutura
    *estrutura onde será guardado o resultado da operação de cifra*/
    pr_info(KERN_INFO "my skcipher is read2");

   

    /* AES 256 with key */
    if (crypto_skcipher_setkey(skcipher, key, 32)) {
        pr_info("key could not be set\n");
        ret = -EAGAIN;
        goto out;
    }

    pr_info(KERN_INFO "my skcipher is read");

    /* IV will be random */
    ivdata = kmalloc(16, GFP_KERNEL);
    if (!ivdata) {
        pr_info("could not allocate ivdata\n");
        goto out;
    }
    sprintf(ivdata, "1234567891234567");
     


    scratchpad = kmalloc(size_of_message, GFP_KERNEL);
    if (!scratchpad) {
        pr_info("could not allocate scratchpad\n");
        goto out;
    }
   sprintf(scratchpad, &message[2]);

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
    ret = test_skcipher_encdec(&sk, message[0]); //função propria do codigo
    if (ret)
        goto out;

    pr_info("Encryption triggered successfully\n");
    buf = sg_virt(&sk.sg);
    sprintf(message, buf);


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
};

//----------------------------------------------------------------------------------//

static int __init cryptomodule_init(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}
	
	//usar a key dentro de um programa que vai conversar com o módulo
	printk(KERN_INFO "my secret key is: %s\n", key);
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	
	// Register the device class
   criptoClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(criptoClass)){                // Check for error and clean up if there is
      unregister_chrdev(Major, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(criptoClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "Cripto: device class registered correctly\n");
 
   // Register the device driver
   criptoDevice = device_create(criptoClass, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
   if (IS_ERR(criptoDevice)){               // Clean up if there is an error
      class_destroy(criptoClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(Major, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(criptoDevice);
   }

	return 0;
}

static void __exit cryptomodule_exit(void)
{
	printk(KERN_INFO "Crypto Module terminated\n");
	device_destroy(criptoClass, MKDEV(Major, 0));     // remove the device
   	class_unregister(criptoClass);                          // unregister the device class
   	class_destroy(criptoClass);                             // remove the device class
   	unregister_chrdev(Major, DEVICE_NAME);             // unregister the major number
   	printk(KERN_INFO "Cripto: Goodbye from the LKM!\n");
}

static ssize_t device_write(struct file *filp,const char *buff,size_t len,loff_t * off)
{
	sprintf(message, "%s(%zu letters)", buff, len);   // appending received string with its length
	size_of_message = strlen(message);                 // store the length of the stored message
	printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);
	
	if(message[0] == 'h')
	{ 
		sprintf(message,"hash");
	}else
	{
		test_skcipher();
	}
	return len;
}

static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
                           char *buffer,        /* buffer to fill with data */
                           size_t length,       /* length of the buffer     */
                           loff_t * offset)
{
	int error_count = 0;
	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count==0){            // if true then have success
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
		return (size_of_message=0);  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
	}
}

static int device_release(struct inode *inode, struct file *file)
{
        Device_Open--;          /* We're now ready for our next caller */

        /*
         * Decrement the usage count, or else once you opened the file, you'll
         * never get get rid of the module.
         */
        module_put(THIS_MODULE);

        return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
        static int counter = 0;

        if (Device_Open)
                return -EBUSY;

        Device_Open++;
        sprintf(msg, "I already told you %d times Hello world!\n", counter++);
        msg_Ptr = msg;
        try_module_get(THIS_MODULE);

        return 0;
}


module_init(cryptomodule_init);
module_exit(cryptomodule_exit);

MODULE_LICENSE("GPL");
