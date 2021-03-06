/** @file */
/*
 * Copyright (c) 2020, Cisco Systems, Inc.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/cisco/libacvp/LICENSE
 */

//
// Created by edaw on 2019-01-07.
//

#include "ut_common.h"
#include "acvp_lcl.h"

ACVP_CTX *ctx;
ACVP_TEST_CASE *test_case;
ACVP_HMAC_TC *hmac_tc;
ACVP_RESULT rv;

int initialize_hmac_tc(ACVP_HMAC_TC *hmac_tc, int alg_id, char *msg, int msg_len, char *key, int key_len, int corrupt) {
    memset(hmac_tc, 0x0, sizeof(ACVP_HMAC_TC));
    
    if (!corrupt) {
        hmac_tc->mac = calloc(1, ACVP_HMAC_MAC_BYTE_MAX);
        if (!hmac_tc->mac) { return -1; }
    }
    
    if (msg) {
        hmac_tc->msg = calloc(1, ACVP_HMAC_MSG_MAX);
        if (!hmac_tc->msg) { return -1; }
        rv = acvp_hexstr_to_bin(msg, hmac_tc->msg, ACVP_HMAC_MSG_MAX, NULL);
        if (rv != ACVP_SUCCESS) {
            ACVP_LOG_ERR("Hex converstion failure (msg)");
            return -1;
        }
    }
    
    if (key) {
        hmac_tc->key = calloc(1, ACVP_HMAC_KEY_BYTE_MAX);
        if (!hmac_tc->key) { return -1; }
        rv = acvp_hexstr_to_bin(msg, hmac_tc->key, ACVP_HMAC_KEY_BYTE_MAX, NULL);
        if (rv != ACVP_SUCCESS) {
            ACVP_LOG_ERR("Hex converstion failure (msg)");
            return -1;
        }
    }
    
    hmac_tc->msg_len = msg_len / 8;
    hmac_tc->key_len = key_len / 8;
    hmac_tc->cipher = alg_id;
    
    return 1;
}

void free_hmac_tc(ACVP_HMAC_TC *hmac_tc) {
    if (hmac_tc->msg) free(hmac_tc->msg);
    if (hmac_tc->mac) free(hmac_tc->mac);
    if (hmac_tc->key) free(hmac_tc->key);
    memset(hmac_tc, 0x0, sizeof(ACVP_HMAC_TC));
}

/*
 * missing msg in hmac tc test case
 */
Test(APP_HMAC_HANDLER, missing_msg) {
    char *msg = NULL;
    char *key = "aaaa";
    hmac_tc = calloc(1, sizeof(ACVP_HMAC_TC));
    
    if (!initialize_hmac_tc(hmac_tc, ACVP_HMAC_SHA1, msg, 8, key, 16, 0)) {
        cr_assert_fail("hash init tc failure");
    }
    test_case = calloc(1, sizeof(ACVP_TEST_CASE));
    test_case->tc.hmac = hmac_tc;
    
    rv = app_hmac_handler(test_case);
    cr_assert_neq(rv, 0);
    
    free_hmac_tc(hmac_tc);
    free(hmac_tc);
    free(test_case);
}

/*
 * missing key in hmac tc test case
 */
Test(APP_HMAC_HANDLER, missing_key) {
    char *msg = "aaaa";
    char *key = NULL;
    hmac_tc = calloc(1, sizeof(ACVP_HMAC_TC));
    
    if (!initialize_hmac_tc(hmac_tc, ACVP_HMAC_SHA1, msg, 8, key, 16, 0)) {
        cr_assert_fail("hash init tc failure");
    }
    test_case = calloc(1, sizeof(ACVP_TEST_CASE));
    test_case->tc.hmac = hmac_tc;
    
    rv = app_hmac_handler(test_case);
    cr_assert_neq(rv, 0);
    
    free_hmac_tc(hmac_tc);
    free(hmac_tc);
    free(test_case);
}

/*
 * the pointer for mac should be allocated
 * by the library. here we don't allocate it and test
 * to see if the handler gracefully handles it
 */
Test(APP_HMAC_HANDLER, disposition_mem_not_allocated) {
    char key[] = "aaaa";
    char msg[] = "64FC1AB2AE132D2AC7DEB5BF31986631F10B11B882893BD2FD859C11E779F953346CB92166C2581C075DED6F78DC54B34B05E6393D11B31D1D8074062F3488B54D2E9C864BB236D7454FCA36A75953718CCC466F2F4681D1F204D39D999D6BD19395C8CB4DE17F92741580BFE2A622F11D0EA49B061B4B16F9EA67CA44ABEDC50EADDCCA4A708030674D8A7A5D7BBB59CE184887637A7E705F133CCE11E6E0FF90DE3A9DFFF10965960715858BAA2A35B878B024AA7DA4494ADCBB54D616377DD3162587B173D27F3DF30507C4C94C1A6712E44963B316B565EC11D17046599B0ED94FD8C591489CC214D8C13209365CAD2B9A2FB1465722EFEA19F90F949415B43711FA26FA4601429590BBD6059F7FE62165045B0DD320D475D20C938A3252CFAEC7D2881C7D1330F53D84662B98C1F6AC8442E663C47B0A9A16A30D77F77F1AF7E66FC7962C6A634C682089DDBAEA19E0838B0A4C49B61E8E68B486E1609915C7622116B2A179388E8F3019E20A734F9D856D100D477C18FC21691064F8F27D8202945009889E4D939951C962B240F878197FD4DD1C185BB22A9E51EF0DD1DF5892CC2202EB681D594B2C26A22F429A72591D36D84B104F25973C71A5F799BFBFF4F8A44DB251964F581FEE6B3EF3EF9C434888CC91D3513719D55482B77579399764961EDEDEBBE2EAC7AC9C4868FAEBBE291A71D257248A17832B6E33801604BC36D338DEAEC55EBA5B24B55E68BB44438802CE79356F6B9D74CCA43C753D0E7D0A0504AD3E73C1F894A742C2C2FFE32867E4718E940EC23A8177526FBEF2619EE6775D4CE4AC28F9916388F8AAEC15C73C632FD7DCF5F56752E85A676D535B1C9FEDAFD7C61F8B97BB4D8489761423DBDEC0C21A0F008B964F246C6F43E34615ECFF0DCDC8F488FE7D3ED7678B448CBE589E4AB469481BACBA77CB4F1E08D593B50FCE02D37B7A29742B622AAD460D1B738B781977FC25BB01E149421EFC2AE06985AE9960FF4AD1B7C1B08E031D8D03B9EA4B48E83E1863A92E021D4C31EB25BA66483B4AC705BFD361578F5FCFEA03FE95730922E446B1C0F86166BE1D320E510F5EED218C992DF46E2D2FEFD95B0A94C03B7F403D3518546AC8E4F5F92D01914769BAD97FC43A15B34757FD9B1B89DC565FEC1FC455BACA376CE6C7AEE13BC52ACB0B623860F4AC5B4395F8FF50C1508F92D734F32C5C52CBF23A73719559493F574D5D6C186F2D94DF4B21A886BD71E0585C072B7A1D7828787BA78E898CA6643E7EBDC2A2A8EC9A02AF10A485CB479AC9FDFE3CBD88BE140FFF15050A3043F0EFA673CE573BDE5699C5B3383287288D80B1BFCD7C960BB24C15FD80FE8560B5E7422BAD58C74B88CB7AD2C93A0B2A0C80148A6AE62B321B54CC95A3C3A9D06B8FED435C94A8A10B1CA5F9AFB15F7FE2B9C12ACFF1EFA32B9FC056FAD2682DC42A8329B9EDE3CDAAE37B5A8A56D1DBABC7680BF16DFAEFB8A7303F62538586762F4F818C0B464AC4B297CAE0A84B5BDE22AA7F59DF5D14AB10340038A88042AE5C03D5B2B0FE5547C1EA4F81A6FFEDA769E7E29633E37A0502318A618D0CAF011970DF96F3C5C60540B33C596216DD9B968E4457825B1F3C99C977760396FAFE44B198839A8335AD3F65F2521F14CE7AA88EF7880AF4B3B1F47A1A2973056A32065C6D319085298D164F4C360D13B6B3F2EA7C6EC9015AB2E32909C4114CFD8B9BFF65D6C8C06524FCC18965A696DAFD3F2B76709105326C3236F2F6AA264FA0B5F3CCE810F6DF685B0D73FC6D8CE8D51FB3EA60EDB37E68E876CCA729C03270BDF7F28F6CFAB02A8C6323106D1BE2FEDD1BB7DC960D98A78239E2C1206BCB7F1EE117508BC6F0D690806E855A01A1CB38ED13F5405051D5477E565DD041ABFAE97E110FE71AECF110F72DE7E2E026D896B302CB66B5FE30F0FE6B77B801CDBE68516581DA377E3AB87AAA3E8042D4B3F60E5E9EFF134B4F00EB476555A46BBB36AE2AB7931CE6F45102DFB204FE1B3764C11C23BC046C9208F99EC02CD03D972CAE3D9C7A397413311AA26CA654489EAC7336923C9869744BA4F10730180FF46E86064C9CC8AB9A5359BB2678B737FA00CED93CC4AB1E33FBDA6525D4C76CED401B24DE4E486C85AFFA3E5ACB654EB0CAC5CF9C05500F92645595094D5B9592B3878BC76801C0CF6BDE8CD33D828990E4B380695A2074CCE28CDF0E18D7643CCE7CDC54F632634B2F29BD7A1CF11E4364AEE1B297630E90DB81F9CB4CEFF92859A8C55CC5ED3FC56518CD68B1504DDC572BFE11FFBEE493D8A24235F8C863FC916ECB6ACE766F62C0E1707CF7E6321DB7F294AFEC8B96251393D23450FFA7D694C44EE46E05CEECDBEA9D96E3F827ABD792349F9FF7C1E052396F9C624A4D5D7754741E9109B0EB9133A7AD6BE85E822F347F9E5C1E2A08E3E0EE1EF05F1F8298087E427A57DF563091BEC9FFA4C1405860FC81CDE566F696EE767160E17D50FDA6522D28B7E32CB64C84C215C0F759524EFF6EB01C29EA36A7F95D8396A3B453D38AF6FE138E58CEAC9EF6D09C9D8F4CFA080396D024879D5041B70FB1D85543545EB24F43535B51963ED4A4C73B775E080F5CECE609B286DA3A526FE0EE0D8142BD0B13FC985B56F5E99F83AB61D8A3B097FDBE5998006072AFE89FAA503B35C27F059FDE27F606CEB685E6692435C2D9FCBF1B23CF00485DEA3BF29E2A0C7AD009A7E023B2A06FB3218E5AE05D4BADA186C9B7CA0B3E696028517E97AA285C116644B4D2D749A3B38A2762396171AE69D29CC7861DE015BE7AB86C5554F9DB84C7BC520CB4803E4D50D5AB21FDE41895A7D29185AC23CDB85169531596F3893549601D7A06563C642273AAEB996627CDCD0349DD2F24E4E4365862F1E560C1FAC0448725B1E698F0B0C0AA91080411B35A387658EE4979D185CF619F47EC24CD05B3EFAA1A8205445C3726A8F569B498F8A9DD092DC7ADAFEEA2A7B72BF528D669A0FBBC9C095D3841A41F9BF968732D868C946FC38D6E00708BB378BB47E87CD3201069973A248D19A1979CC71D3BA21A63FE1371FCF7790A92F7C19677DAD940E7B9350A8D9085A0056CBA077893674C4AEF952028638D551BF727BFC593855C9F145A8588ED09F81F3CD4894C9501B9B78629FC367854423FBDF79788A5DCDAF5079C794C1ED274EBCC997DA6CC13138FB065A16D7B633B8DA7A45B61F5C4B9B97A5660A890691C2797789AB4E2EAE319598D4D5E96554C67450AB63BFA0B64664D5F90B02C6C1CFDC338C3C8CB81985EE0566D592B07190B078C15AD67E0605A4066357A48C36FC6E77865AF6053F45AB71D1C5A66A4CC83DAD8749B33047629E8241CC0DC4F7CFCB1D4FEF88AF3E47B2848B2DD0A487E5C2612CAC5C0209AE571F0A344CF1CA269FB26A3E500F395A49D34BCCDEAE71A8B68EBDC7EB9772254B9FEEB209882B0E4159CC524D4E7470390843CE27454E35C113CC68001B806C04A77CD48F62629DA173122CE432BC82F6BDE37BF386DCB8746E147C15DC716386A08509C17B3E1C843754F40DB2A3E58FE234717F170C99A14E5FBD8EF6C506672FEF88284C753B3F628300107E06C41BA362EAD2E8836628AD4646B67D8F173775721486CF88F2DD176A027D3546F785063B0E0E0C7BE9346647A14F42529965003567658BCA56A170E75EC1B7CC705E95890916E4177CDCE7A041163A73C4662BBA49A5A58D0E1F41F2257958C0D4B75DEBB306B8C52F59DB0CF299D998A39660A7928C83CE4A488FC1EC036147D9DA3A2AAEDE0A250DCE46CD73EFC59DD12E0596C132AB1ED20CC9CD2BC68366175EECC827BAC6A92D21DA3D9B55DB0405AB887F7988A6C69261D575873756E7FBE2235EE3DB39699D7F1F67F93D70B77240F472BE8F484AADD093611454184C99D37B1C1D5D44FA2E46FEC1D3449725C2E9D9AF73E0CB66FE30AC38D567A09C30E2AC906343A14BE0786379E6468B4A0968D8B4F0593F3673DE5FBEA";
    hmac_tc = calloc(1, sizeof(ACVP_HMAC_TC));
    
    if (!initialize_hmac_tc(hmac_tc, ACVP_HMAC_SHA1, msg, 22688, key, 16, 1)) {
        cr_assert_fail("hash init tc failure");
    }
    test_case = calloc(1, sizeof(ACVP_TEST_CASE));
    test_case->tc.hmac = hmac_tc;

    rv = app_hmac_handler(test_case);
    cr_assert_neq(rv, 0);
    
    free_hmac_tc(hmac_tc);
    free(hmac_tc);
    free(test_case);
}
