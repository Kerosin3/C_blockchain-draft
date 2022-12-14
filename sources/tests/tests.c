#include "tests.h"
#include <assert.h>
#include <sodium/crypto_generichash.h>
#include <sodium/crypto_sign.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>

int test_hpoint_from_2msg_creation();
int test_hashG_node_creation();
int test_process_messages();
int create_test_messages(unsigned long long);
int test_mekrle_proof_RIGHT();
int test_mekrle_proof_WRONG();
int test_cleanup_message();
int test_message_creation_and_validation();
int test_create_and_destroy_hpoint();
int test_process_messages_L1_v2();

void tests()
{
    int result = 0;
        result+=test_valid_messages(); // no memory safe
        result+=test_INvalid_messages(); // no memory safe
        result+=test_hash(); // no memry safe
        result+=test_hash_merging();

        result += test_cleanup_message();
        result += test_message_creation_and_validation();
        result += test_create_and_destroy_hpoint();
        result += test_process_messages_L1_v2();

        result += create_test_messages(9);
        result += test_mekrle_proof_RIGHT();
        result += test_mekrle_proof_WRONG();
        printf("passed %d tests\n",result);
    (!result) ? printf("ALL TESTS PASSED OK\n") : printf("SOME ERRORS WHILE TESTING OCCURRED!\n");
}

int test_mekrle_proof_RIGHT()
{
    unsigned long long EXPONENT = 9;
    //-----create basic structures
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free

    user_keys uk = create_key_pair();
    signed_message_t *wrong_message = ls_get_a_signed_msg(uk); // pointer to message

    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        validate_a_message(*msg_arr[i], uk.pk);
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT - 1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1]; // store pointer
    n_msg >>= 1;                                        // devide by 2
    //--------------------------
    // create intermideate layers
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    // DumpHex(((*(L_arrays_p[0].main_pointer))->hash), crypto_generichash_BYTES);
    /*int wreck = 1;
    if (wreck)  memcpy(
            ((*(L_arrays_p[EXPONENT - 1].main_pointer[125])).messages.smsg_p1)->message,
            wrong_message->message,
            wrong_message->length  );
    */
    unsigned long long ver_result = 0;
    for (size_t i = 0; i < (1LLU << EXPONENT); i++)
    { // verify all messages
        // printf("----------------->verify %lu nth\n",i);
        int rez = merkle_verify_message(EXPONENT, i, L_arrays_p);
        if (!rez)
            break;
        ver_result += rez;
    }
    // free layers data
    //  free rootlevel
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
    for (i = 0; i < (1LLU << EXPONENT); i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages
    destroy_signed_message(wrong_message);
    if (ver_result != (1LLU << EXPONENT))
        return 1;
    return 0;
}

// CREATE TREE TEST TREE AND CLEAN
int create_test_messages(unsigned long long EXPONENT)
{
    //-----create basic structures
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free
    printf("creating test n msg :%llu\n", n_msg);

    user_keys uk = create_key_pair();
    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        validate_a_message(*msg_arr[i], uk.pk);
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT - 1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1]; // store pointer
    n_msg >>= 1;                                        // devide by 2
    //--------------------------
    // create intermideate layers
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    //
    // free rootlevel
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
    //     destoroy_a_layer(&L_arrays_p[EXPONENT-1]); // destroy level
    for (i = 0; i < (1LLU << EXPONENT); i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages
    return 0;      // no segfault = all is ok
}

// ok!
int test_message_creation_and_validation()
{

    user_keys uk = create_key_pair();
    signed_message_t **msg_arr = calloc(512, sizeof(signed_message_t *));
    size_t i = 0;
    int rez = 0;
    for (i = 0; i < 512; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk);
        rez += validate_a_message(*msg_arr[i], uk.pk);
    }
    //     printf("rez is %d\n", rez);

    for (i = 0; i < 512; i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr);
    return (rez == 512) ? 0 : 1;
}

// ok!
int test_cleanup_message()
{

    user_keys uk = create_key_pair();
    signed_message_t *msg;
    msg = ls_get_a_signed_msg(uk); // generate random
                                   //     DumpHex(msg->message, msg->length);
    validate_a_message(*msg, uk.pk);
    destroy_signed_message(msg);
    return 0; // no segfault = ok;
}

// ok!
int test_create_and_destroy_hpoint()
{

    user_keys uk = create_key_pair();
    signed_message_t *msg1;
    msg1 = ls_get_a_signed_msg(uk); // generate random
                                    //     printf("message 1\n");
                                    //     DumpHex(msg1->message, msg1->length);
    validate_a_message(*msg1, uk.pk);

    user_keys uk2 = create_key_pair();
    signed_message_t *msg2;
    msg2 = ls_get_a_signed_msg(uk2); // generate random
                                     //     printf("message 2\n");
                                     //     DumpHex(msg2->message, msg2->length);
    validate_a_message(*msg2, uk2.pk);

    hash_point_p HASH_POINT = 0;
    HASH_POINT = create_hpoint_message(msg1, msg2);
    int rez = 0;
    if (HASH_POINT->messages.smsg_p1 == msg1)
    {
        rez++;
    }
    if (HASH_POINT->messages.smsg_p2 == msg2)
    {
        rez++;
    }
    destroy_signed_message(msg1);
    destroy_signed_message(msg2);
    destroy_hpoint_message(HASH_POINT);
    return (rez == 2) ? 0 : 1;
}
// OK
int test_process_messages_L1_v2()
{
    unsigned long long n_msg = (1LLU << 5LLU); //  create 2^9 messages
    user_keys uk = create_key_pair();
    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    size_t i = 0;
    unsigned long long rez = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        rez += validate_a_message(*msg_arr[i], uk.pk);
    }
    layer_hp *root_layer = process_s_messagesV2(n_msg, msg_arr);

    destoroy_a_layer(root_layer); // destroy level
    for (i = 0; i < n_msg; i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages

    return (rez == n_msg) ? 0 : 1;
}
int test_hashG_node_creation()
{
    // create 1 ground node with 2 msg
    user_keys uk = create_key_pair();
    signed_message_t *a_msg = get_a_signed_msg(uk);
    printf("message 1 is \n");
    DumpHex(a_msg->message, a_msg->length);
    printf("\n");

    signed_message_t *a_msg2 = get_a_signed_msg(uk);

    if (!validate_a_message(*a_msg, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg2, uk.pk))
        printf("validation error!\n");

    hash_point_p aHP1 = create_hpoint_message(a_msg, a_msg2); // created
                                                              //

    // create 2 ground node with 2 msg
    signed_message_t *a_msg3 = get_a_signed_msg(uk);

    signed_message_t *a_msg4 = get_a_signed_msg(uk);

    if (!validate_a_message(*a_msg3, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg4, uk.pk))
        printf("validation error!\n");

    hash_point_p aHP2 = create_hpoint_message(a_msg3, a_msg4); // created
                                                               //
    // crate G node
    hash_point_p GNODE = create_hpoint_hashG(aHP1, aHP2);
    printf("test message 1 from GNODE\n");
    if ((((hash_point_p)(GNODE->hpoint1))->messages.smsg_p1) == a_msg)
    {
        printf("validated msg1\n");
    }
    else
    {
        printf(" msg1 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint1))->messages.smsg_p2) == a_msg2)
    {
        printf("validated msg2\n");
    }
    else
    {
        printf(" msg2 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint2))->messages.smsg_p1) == a_msg3)
    {
        printf("validated msg3\n");
    }
    else
    {
        printf(" msg3 invalid");
    }
    if ((((hash_point_p)(GNODE->hpoint2))->messages.smsg_p2) == a_msg4)
    {
        printf("validated msg4\n");
    }
    else
    {
        printf(" msg4 invalid");
    }
    return 1;

    // DumpHex( ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p)
    // (GNODE->hpoint1))->messages.smsg_p1->length ); printf("test message 2 from GNODE\n"); DumpHex( ((hash_point_p)
    // (GNODE->hpoint1))->messages.smsg_p1->message ,  ((hash_point_p) (GNODE->hpoint1))->messages.smsg_p1->length );

    return 1;
}

int test_hpoint_from_2msg_creation()
{

    user_keys uk = create_key_pair();
    signed_message_t *a_msg = get_a_signed_msg(uk);
    printf("message is\n");
    DumpHex(a_msg->message, a_msg->length);

    user_keys uk2 = create_key_pair();
    signed_message_t *a_msg2 = get_a_signed_msg(uk2);

    if (!validate_a_message(*a_msg, uk.pk))
        printf("validation error!\n");

    if (!validate_a_message(*a_msg2, uk2.pk))
        printf("validation error!\n");

    hash_point_p aHP1 = create_hpoint_message(a_msg, a_msg2);

    printf("check:\n");
    DumpHex(aHP1->messages.smsg_p1->message, aHP1->messages.smsg_p1->length);
    if ((a_msg) == aHP1->messages.smsg_p1)
    {
        printf("valid!\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

int test_mekrle_proof_WRONG()
{
    unsigned long long EXPONENT = 14;
    //-----create basic structures
    unsigned long long n_msg = (1LLU << EXPONENT); //  create 2^9 messages
    layer_hp *L_arrays[EXPONENT];
    layer_hp L_arrays_p[EXPONENT]; // for free
    printf("verefying n msg, test wrong :%llu\n", n_msg);

    user_keys uk = create_key_pair();
    signed_message_t *wrong_message = ls_get_a_signed_msg(uk); // pointer to message

    signed_message_t **msg_arr = calloc(n_msg, sizeof(signed_message_t *));
    //----fill messages
    size_t i = 0;
    for (i = 0; i < n_msg; i++)
    {
        msg_arr[i] = ls_get_a_signed_msg(uk); // pointer to message
        validate_a_message(*msg_arr[i], uk.pk);
    }
    //-----------------------------------
    // CREATE BASE LAYER
    L_arrays[EXPONENT - 1] = process_s_messagesV2(n_msg, msg_arr);
    L_arrays_p[EXPONENT - 1] = *L_arrays[EXPONENT - 1]; // store pointer
    n_msg >>= 1;                                        // devide by 2
    //--------------------------
    // create intermideate layers
    fill_intermediate_levels(EXPONENT, &n_msg, L_arrays, L_arrays_p); // done

    // DumpHex(((*(L_arrays_p[0].main_pointer))->hash), crypto_generichash_BYTES);//merkle root
    int wreck = 1; // <<----------------- FALSE MESSAGE HERE!!!!!
    if (wreck)
        memcpy(((*(L_arrays_p[EXPONENT - 1].main_pointer[125])).messages.smsg_p1)->message, // 125 = 250 msg
               wrong_message->message, wrong_message->length);

    int ver_result = 0;
    for (size_t i = 0; i < (1LLU << EXPONENT); i++)
    { // verify all messages
        // printf("----------------->verify %lu nth\n",i);
        int rez = merkle_verify_message(EXPONENT, i, L_arrays_p);
        if (!rez)
            break;
        ver_result += rez;
    }
    // free layers data
    //  free rootlevel
    for (size_t i = 0; i < EXPONENT; i++)
    {
        destoroy_a_layer(L_arrays[i]);
    }
    for (i = 0; i < (1LLU << EXPONENT); i++)
    {
        destroy_signed_message(msg_arr[i]);
    }
    free(msg_arr); // free conrainer for messages
    destroy_signed_message(wrong_message);
    if (ver_result != (125 * 2))
        return 1;
    return 0;
}

int test_hash_merging()
{

    printf("TEST HASHIN MERGING\n");
    user_keys uk = create_key_pair();
    srand(time(NULL));
    test_msg_t somemsg = get_test_msg(100);
    signed_message_t a_msg = sign_a_message(somemsg, uk.sk);

    user_keys uk2 = create_key_pair();
    test_msg_t somemsg2 = get_test_msg(100);
    signed_message_t a_msg2 = sign_a_message(somemsg2, uk2.sk);

    merge_2hashses(calc_hash(a_msg), calc_hash(a_msg2));
    return 0; // mo segfault == ok
}
// test hash generating
int test_hash()
{
    printf("TEST HASHING FUNCTION\n");
    user_keys uk = create_key_pair();
    srand(time(NULL));
    test_msg_t somemsg = get_test_msg(100);
    signed_message_t a_msg = sign_a_message(somemsg, uk.sk);
    unsigned char hash[crypto_generichash_BYTES];
    unsigned char hash_re[crypto_generichash_BYTES];
    memcpy(hash, calc_hash(a_msg), crypto_generichash_BYTES);
    //     printf("calced hash:\n");
    //     DumpHex(hash,crypto_generichash_BYTES);
    size_t i = 0;
    for (i = 0; i < 500; i++)
    {
        //		printf("i=%ld new hash:\n",i);
        memcpy(hash_re, calc_hash(a_msg), crypto_generichash_BYTES);
        //		DumpHex(hash_re,crypto_generichash_BYTES);
        if ((strncmp((char *)hash, (char *)hash_re, crypto_generichash_BYTES)) != 0)
            break;
    }
    if (i == 500)
    {
        //    printf("TEST PASSED, NO ERROR OCCURRED \n");
        return 0;
    }
    // else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
    return 1;
}
// test message encryption
int test_valid_messages()
{
    printf("TESTING VALID ENCTYPTION\n");
    user_keys uk = create_key_pair();
    signed_message_t a_msg;
    size_t i = 0;
    for (i = 0; i < 15; i++)
    {
        int result = 0;
        test_msg_t somemsg = ls_get_test_msg(15);
        // DumpHex(somemsg.test_msg, somemsg.len);
        a_msg = sign_a_message(somemsg, uk.sk);
        for (size_t ii = 0; ii < 100; ii++)
        {
            //---validation
            ((validate_a_message(a_msg, uk.pk))) ? (result = 1) : (result = 0);
            if (!result)
                break;
        }
        //	    printf("signed!\n");
        //            (result==1) ? printf("validated!\n") : printf("forged!\n");
        if (!result)
            break;
    }
    if (i == 15)
    {
        //      printf("TEST PASSED, NO ERROR OCCURRED \n");
        return 0;
    }
    // else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
    return 1;
}

// wrong keys
int test_INvalid_messages()
{
    printf("TEST FORGED MESSAGE DETECTON\n");
    user_keys uk = create_key_pair();
    signed_message_t a_msg;
    // char* somemsg = "asdfghj";

    srand(time(NULL));
    test_msg_t somemsg[15];
    for (size_t k = 0; k < 15; k++)
    {
        somemsg[k] = ls_get_test_msg(15);
    }

    int result = 0;
    for (size_t i = 0; i < 15; i++)
    {
        a_msg = sign_a_message(somemsg[i], uk.sk);
        //---validation
        ((validate_a_message(a_msg, uk.pk))) ? (result += 1) : (result = 0);
        //      (result==1) ? printf("validated!\n") : printf("forged!\n");
    }

    if (result == 15)
    {
        //    printf("TEST PASSED, NO ERROR OCCURRED \n");
        return 0;
    }
    // else printf("TEST PASSED, ERROR OCCURRED!!!!!!!!!!!!! \n");
    return 1;
}
