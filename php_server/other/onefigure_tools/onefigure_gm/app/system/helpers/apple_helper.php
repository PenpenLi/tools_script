<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');

function ValidateAppleAppStoreSignature($receipt, $is_sandbox) {
    //$sandbox should be TRUE if you want to test against itunes sandbox servers
    if ($is_sandbox)
        $verify_host = "ssl://sandbox.itunes.apple.com";
    else
        $verify_host = "ssl://buy.itunes.apple.com";

    $json='{"receipt-data" : "'.$receipt.' }';
    //opening socket to itunes
    $fp = fsockopen ($verify_host, 443, $errno, $errstr, 30);
    if (!$fp)
    {
        // HTTP ERROR
        return 0;
    }
    else
    {
        //iTune's request url is /verifyReceipt
        $header = "POST /verifyReceipt HTTP/1.0\r\n";
        $header .= "Content-Type: application/x-www-form-urlencoded\r\n";
        $header .= "Content-Length: " . strlen($json) . "\r\n\r\n";
        fputs ($fp, $header . $json);
        $res = '';
        while (!feof($fp))
        {
            $step_res = fgets ($fp, 1024);
            $res = $res . $step_res;
        }
        fclose ($fp);
        //taking the JSON response
        $json_source = substr($res, stripos($res, "\r\n\r\n{") + 4);
        //decoding
        $app_store_response_map = json_decode($json_source);
        $app_store_response_status = $app_store_response_map->{'status'};
        if ($app_store_response_status == 0)//eithr OK or expired and needs to synch
        {
            //here are some fields from the json, btw.
            $json_receipt = $app_store_response_map->{'receipt'};
            $transaction_id = $json_receipt->{'transaction_id'};
            $original_transaction_id = $json_receipt->{'original_transaction_id'};
            $json_latest_receipt = $app_store_response_map->{'latest_receipt_info'};
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
