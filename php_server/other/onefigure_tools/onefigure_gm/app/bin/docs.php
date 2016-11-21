<?php
$docs = array(
/*
        'ofdp' => array(
                'user_level' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=4&output=csv',
                'stage' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=3&output=csv',
                'gambling' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=11&output=csv',
                'mission' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=12&output=csv',
                'league_reward' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=17&output=csv',
                'attendance' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=19&output=csv',
                'iap' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=16&output=csv',
                'price' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=7&output=csv',
                'item' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=6&output=csv',
                'enchant' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=20&output=csv',
                'enchant_rate' => 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=9&output=csv',
            ),
        'spmc' => array(
                'card' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=2&output=csv',
                'level' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=12&output=csv',
                'mission' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=15&output=csv',
                'upgrade' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=8&output=csv',
                'attendance' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=17&output=csv',
                'potion' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=13&output=csv'
            ),
        'spmc-global' => array(
                'card' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=2&output=csv',
                'level' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=12&output=csv',
                'mission' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=15&output=csv',
                'upgrade' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=8&output=csv',
                'attendance' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=17&output=csv',
                'potion' => 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=13&output=csv'
            )
*/
    );

$limit = 100;

for ($i = 0; $i < $limit; $i++) {
    $docs['ofdp/development'][$i] = 'https://docs.google.com/spreadsheet/pub?key=0AjeOl3MuPwCgdG9WQXBxRmRvOTl0Z0NsVzhSOXB6OHc&single=true&gid=' . $i . '&output=csv';
}

for ($i = 0; $i < $limit; $i++) {
    $docs['ofdp/service'][$i] = 'https://docs.google.com/spreadsheet/pub?key=0AkYtN686o8fpdDQyZ085WkVuRE5INnNqZkF5OGRZc1E&single=true&gid=' . $i . '&output=csv';
}

for ($i = 0; $i < $limit; $i++) {
    //$docs['spmc'][$i] = 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdHdlWks3b3BNMTRZUzBaZmpqR0hsTlE&single=true&gid=' . $i . '&output=csv';
}

for ($i = 0; $i < $limit; $i++) {
    //$docs['spmc-global'][$i] = 'https://docs.google.com/spreadsheet/pub?key=0AqljFV5d3xsOdFRmN0d3ZmZkbWdKbDZnMktaODBRaXc&single=true&gid=' . $i . '&output=csv';
}

foreach ($docs as $property => $value) {
    foreach ($value as $type => $url) {
        $tmp_file = '/tmp/' . str_replace('/', '.', $property) . '.' . $type . '.csv';

        $command = '/usr/bin/curl --connect-timeout 5 "' . $url . '" > ' . $tmp_file . '&';
        exec($command);
    }
}

sleep(30);

foreach ($docs as $property => $value) {
    foreach ($value as $type => $url) {
        $tmp_file = '/tmp/' . str_replace('/', '.', $property) . '.' . $type . '.csv';

        $contents = file_get_contents($tmp_file);

        if (filesize($tmp_file) > 0 && substr($contents, 0, 1) != '<') {
            $command = '/bin/cat "' . $tmp_file . '" > /home/nginx/html/images/Purple_OFDP/docs/' . $property . '/' . $type . '.csv';
            exec($command);
        }

        unlink($tmp_file);
    }
}
