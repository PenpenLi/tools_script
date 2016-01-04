<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');

function ValidateGooglePlaySignature($receipt, $signature) {
    $publicGoogleKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAt+zLT00kiP/8iHZrvBwbAOIibC6qhGW9Mt6FHFHh+uJN5+wIYWKfsWS8cU9383iJ6Q0zL2Gk7UQtZvp9ug3yCzWkTADWzepO8rm0+gBuv7OcrIq5TF5qIS4qXrmTg1VkloJb0C4OP9IPqRpa9VKa1nWIa1VbLY2U4U7vgQIcLBIGL+5d2/qhjj4UeK3seWvY8XxHh9CElxMAmaOWU6aNUSon0G7r68gwx15hMOoVy4ICeKrGyn8XibTiruYwXHwBJ6JQNYzWRtJPEF1DL1TLev/DneVVoFgrc6ZnZMZGwlnYLKn0AolCTfq2c1GRUj/FI/wd3Rcm6lHeN3pbkmb1GwIDAQAB";

    $receipt = trim($receipt);
    $signature = trim($signature);

    //Create an RSA key compatible with openssl_verify from our Google Play sig
    $key = "-----BEGIN PUBLIC KEY-----\n".
    chunk_split($publicGoogleKey, 64, "\n").
    '-----END PUBLIC KEY-----';
    $key = openssl_pkey_get_public($key);
//print $signature;print_r(openssl_pkey_get_details($key)); exit();
    if($key == false){
        return $ret = 100;
    }

    //Signature should be in binary format, but it comes as BASE64.
    $signature = base64_decode($signature);

    //Verify the signature
    $result = openssl_verify($receipt, $signature, $key, OPENSSL_ALGO_SHA1);
//print $result . ' / ' . $receipt . ' / ' . $signature . ' / ' . $key;exit();
    if ($result == 1) {
        $ret = 1;
    } elseif ($result == 0) {
        $ret = 0;
    } else {
        $ret = 2;
    }

    return $ret;
}
