/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_test.h"
#include "tomcrypt.h"

/* If you declare any globals in php_test.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(test)
*/

/* True global resources - no need for thread safety here */
static int le_test;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("test.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_test_globals, test_globals)
    STD_PHP_INI_ENTRY("test.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_test_globals, test_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_test_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_test_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	
	unsigned char myKey[32] = "123456";// 加密密钥    
	symmetric_key skey;// 加密状态  
	aes_setup(myKey, 32, 0, &skey);// 加密前初始化状态(密钥，密钥长度，迭代次数(0: 使用推荐值)，加密状态)
	unsigned char c1[128];
	//unsigned char c1[128] = "hello res hello res hello res  res hello res hello  res hello res hello  res hello res hello";// 需要加密的字符串长度任意	
	strcpy(c1,&arg);
	unsigned char c2[128];// 输出长度>= 输入长度，分段长度的倍数(这里就是16的倍数)

	// 分段加密，每段长度为16 (一般不可能只有2段，自己写循环)
	aes_ecb_encrypt(&c1[0], &c2[0], &skey);
	aes_ecb_encrypt(&c1[16], &c2[16], &skey);
	aes_ecb_encrypt(&c1[32], &c2[32], &skey);
	aes_ecb_encrypt(&c1[48], &c2[48], &skey);
	aes_ecb_encrypt(&c1[64], &c2[64], &skey);
	aes_ecb_encrypt(&c1[80], &c2[80], &skey);
	aes_ecb_encrypt(&c1[96], &c2[96], &skey);
	aes_ecb_encrypt(&c1[112], &c2[112], &skey);
	aes_ecb_encrypt(&c1[128], &c2[128], &skey);

	unsigned char c3[128];
	aes_setup(myKey, 32, 0, &skey);// 解密前初始化状态，其实和加密的状态是完全一致的

	// 分段解密     
	aes_ecb_decrypt(&c2[0], &c3[0], &skey);
	aes_ecb_decrypt(&c2[16], &c3[16], &skey);
	aes_ecb_decrypt(&c2[32], &c3[32], &skey);
	aes_ecb_decrypt(&c2[48], &c3[48], &skey);
	aes_ecb_decrypt(&c2[64], &c3[64], &skey);
	aes_ecb_decrypt(&c2[80], &c3[80], &skey);
	aes_ecb_decrypt(&c2[96], &c3[96], &skey);
	aes_ecb_decrypt(&c2[112], &c3[112], &skey);
	aes_ecb_decrypt(&c2[128], &c3[128], &skey);

	len = spprintf(&strg, 0, c3, "test", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_test_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_test_init_globals(zend_test_globals *test_globals)
{
	test_globals->global_value = 0;
	test_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(test)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(test)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(test)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(test)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(test)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "test support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ test_functions[]
 *
 * Every user visible function must have an entry in test_functions[].
 */
const zend_function_entry test_functions[] = {
	PHP_FE(confirm_test_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in test_functions[] */
};
/* }}} */

/* {{{ test_module_entry
 */
zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER,
	"test",
	test_functions,
	PHP_MINIT(test),
	PHP_MSHUTDOWN(test),
	PHP_RINIT(test),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(test),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(test),
	PHP_TEST_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
