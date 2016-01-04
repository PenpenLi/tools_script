<script type="text/javascript">
google.load("visualization", "1", {packages:["corechart"]});

function drawPieChart(title, data_array) {
    var data = google.visualization.arrayToDataTable(data_array);

    var options = {
        'title':title,
        'width':950,
        'height':550,
        'fontSize':11,
        'chartArea':{left:60,top:20,width:800,height:500}
    };

    var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
    chart.draw(data, options);
}

function clickChartButton(title, grid_id) {
    var selected_rows_id = $(grid_id).jqGrid('getGridParam', 'selarrrow');
    var selected_rows = null;

    selected_rows_id.sort();

    if (selected_rows_id.length == 0) {
        alert('데이터를 선택해 주세요.');
        return false;
    }

    var data_array = [['아이템 조합', '플레이수']];

    for (var index in selected_rows_id) {
        selected_rows = $(grid_id).jqGrid('getRowData', selected_rows_id[index]);
        data_array[parseInt(index) + 1] = [selected_rows['ITEMS_NAME'], parseInt(selected_rows['TOTAL_COUNT'])];
    }

    drawPieChart(title, data_array);
    $('#chart-viewer').dialog('open');
}

function showCoinOfLevel() {
    var columns_name = ['Date', 'Gold Sum', 'Ruby Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_GOLD', index:'TOTAL_GOLD', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_RUBY', index:'TOTAL_RUBY', width:80, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    var i = columns_name.length;
    for (var level = 1; level <= 100; level++) {
        columns_name[i] = 'Gold';
        columns_detail[i] = {name:'GOLD_' + level, index:'GOLD_' + level, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Ruby';
        columns_detail[i] = {name:'RUBY_' + level, index:'RUBY_' + level, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;
    }

    $('#grid-area').empty();
    $('<table id="item_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'item', 'ajaxGetCoinOfLevelData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
        repeatitems:false,
        root:"rows"
        },
        width:1000,
        height:"auto",
        toolbar:[false,"top"],
        caption:"Coin average by level"
    });

    var groupHeaderArray = [], index = 0;
    for (level = 1; level <= 100; level++, index++) {
        groupHeaderArray[index] =
            {
                startColumnName:'GOLD_' + level,
                numberOfColumns:2,
                titleText:'Lv.' + level
            };
    }

    $("#item_table").jqGrid('setGroupHeaders', {
        useColSpanStyle:true,
        groupHeaders:groupHeaderArray
    });

    $("#item_table").jqGrid('setFrozenColumns');
    $("#t_item_table")
        //.append('<input type="button" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    //$("input","#t_item_table").click(function() { clickChartButton('아이템 유료 결제수', '#item_table'); });
    $("#click_excel","#t_item_table").click(function() { clickExcelButton(); });
}

function showRuneCountGrid() {
    var RUNE_ARRAY = <?php echo json_encode($RUNE_ARRAY); ?>;
    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    var i = columns_name.length;
    for (var rune_info_id in RUNE_ARRAY) {
        columns_name[i] = RUNE_ARRAY[rune_info_id]['NAME'];
        columns_detail[i] = {name:'RUNE_COUNT_' + rune_info_id, index:'RUNE_COUNT_' + rune_info_id, width:100, align:"right", sortable:false, formatter:'integer'};
        i++;
    }

    $('#grid-area').empty();
    $('<table id="item_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'item', 'ajaxGetRuneCountData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect: false,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        toolbar:[false,"top"],
        caption:"Rune Inventory"
    });

    $("#item_table").jqGrid('setFrozenColumns');
    $("#t_item_table")
        //.append('<input type="button" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    //$("input","#t_item_table").click(function() { clickChartButton('아이템 유료 결제수', '#item_table'); });
    $("#click_excel","#t_item_table").click(function() { clickExcelButton(); });
}

function showLotboxCountGrid() {
    var RUNE_ARRAY = <?php echo json_encode($RUNE_ARRAY); ?>;
    var columns_name = ['Date', 'Hearts', 'Ruby', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'LOT_TYPE1_COUNT', index:'LOT_TYPE1_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'LOT_TYPE2_COUNT', index:'LOT_TYPE2_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    var i = columns_name.length;
    for (var rune_info_id in RUNE_ARRAY) {
        columns_name[i] = 'Hearts';
        columns_detail[i] = {name:'PRODUCT_LOT_TYPE1_COUNT_' + rune_info_id, index:'PRODUCT_LOT_TYPE1_COUNT_' + rune_info_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Ruby';
        columns_detail[i] = {name:'PRODUCT_LOT_TYPE2_COUNT_' + rune_info_id, index:'PRODUCT_LOT_TYPE2_COUNT_' + rune_info_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Rate';
        columns_detail[i] = {name:'PRODUCT_RATE_' + rune_info_id, index:'PRODUCT_RATE_' + rune_info_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;
    }

    $('#grid-area').empty();
    $('<table id="item_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'item', 'ajaxGetLotboxCountData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect: false,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        toolbar:[false,"top"],
        caption:"Lottery box"
    });

    var groupHeaderArray = [], index = 0;
    for (var rune_info_id in RUNE_ARRAY) {
        groupHeaderArray[index] =
            {
                startColumnName:'PRODUCT_LOT_TYPE1_COUNT_' + rune_info_id,
                numberOfColumns:3,
                titleText:RUNE_ARRAY[rune_info_id]['NAME'] + ' (' + rune_info_id + ')'
            };
        index++;
    }

    $("#item_table").jqGrid('setGroupHeaders', {
        useColSpanStyle:true,
        groupHeaders:groupHeaderArray
    });

    $("#item_table").jqGrid('setFrozenColumns');
    $("#t_item_table")
        //.append('<input type="button" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    //$("input","#t_item_table").click(function() { clickChartButton('아이템 유료 결제수', '#item_table'); });
    $("#click_excel","#t_item_table").click(function() { clickExcelButton(); });
}

function showCoinGainCountGrid() {
    var COIN_LOG_INFO_ARRAY = <?php echo json_encode($COIN_LOG_INFO_ARRAY); ?>;
    var columns_name = ['Date', 'Gold Gainned', 'Gold Sum', 'Ruby Gainned', 'Ruby Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_GOLD_COUNT', index:'TOTAL_GOLD_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_GOLD_AMOUNT', index:'TOTAL_GOLD_AMOUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_RUBY_COUNT', index:'TOTAL_RUBY_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_RUBY_AMOUNT', index:'TOTAL_RUBY_AMOUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    var i = columns_name.length;
    for (var log_id in COIN_LOG_INFO_ARRAY) {
        if (log_id > 50) break;

        columns_name[i] = 'Gainned';
        columns_detail[i] = {name:'GOLD_COUNT_' + log_id, index:'GOLD_COUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Gold';
        columns_detail[i] = {name:'GOLD_AMOUNT_' + log_id, index:'GOLD_AMOUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Gainned';
        columns_detail[i] = {name:'RUBY_COUNT_' + log_id, index:'RUBY_COUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Ruby';
        columns_detail[i] = {name:'RUBY_AMOUNT_' + log_id, index:'RUBY_AMOUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;
    }

    $('#grid-area').empty();
    $('<table id="item_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'item', 'ajaxGetCoinData', 1)); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect: false,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        toolbar:[false,"top"],
        caption:"Daily gainned coin amout"
    });

    var groupHeaderArray = [], index = 0;
    for (var log_id in COIN_LOG_INFO_ARRAY) {
        if (log_id > 50) break;

        groupHeaderArray[index] =
            {
                startColumnName:'GOLD_COUNT_' + log_id,
                numberOfColumns:4,
                titleText:COIN_LOG_INFO_ARRAY[log_id]
            };
        index++;
    }

    $("#item_table").jqGrid('setGroupHeaders', {
        useColSpanStyle:true,
        groupHeaders:groupHeaderArray
    });

    $("#item_table").jqGrid('setFrozenColumns');
    $("#t_item_table")
        //.append('<input type="button" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    //$("input","#t_item_table").click(function() { clickChartButton('아이템 유료 결제수', '#item_table'); });
    $("#click_excel","#t_item_table").click(function() { clickExcelButton(); });
}

function showCoinUseCountGrid() {
    var COIN_LOG_INFO_ARRAY = <?php echo json_encode($COIN_LOG_INFO_ARRAY); ?>;
    var columns_name = ['Date', 'Gold Used', 'Gold Sum', 'Ruby Used', 'Ruby Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_GOLD_COUNT', index:'TOTAL_GOLD_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_GOLD_AMOUNT', index:'TOTAL_GOLD_AMOUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_RUBY_COUNT', index:'TOTAL_RUBY_COUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true},
            {name:'TOTAL_RUBY_AMOUNT', index:'TOTAL_RUBY_AMOUNT', width:80, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    var i = columns_name.length;
    for (var log_id in COIN_LOG_INFO_ARRAY) {
        if (log_id < 50) continue;

        columns_name[i] = 'Used';
        columns_detail[i] = {name:'GOLD_COUNT_' + log_id, index:'GOLD_COUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Gold';
        columns_detail[i] = {name:'GOLD_AMOUNT_' + log_id, index:'GOLD_AMOUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Used';
        columns_detail[i] = {name:'RUBY_COUNT_' + log_id, index:'RUBY_COUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;

        columns_name[i] = 'Ruby';
        columns_detail[i] = {name:'RUBY_AMOUNT_' + log_id, index:'RUBY_AMOUNT_' + log_id, width:80, align:"right", sortable:false, formatter:'integer'};
        i++;
    }

    $('#grid-area').empty();
    $('<table id="item_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'item', 'ajaxGetCoinData', 2)); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect: false,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        toolbar:[false,"top"],
        caption:"Daily used coin amount"
    });

    var groupHeaderArray = [], index = 0;
    for (var log_id in COIN_LOG_INFO_ARRAY) {
        if (log_id < 50) continue;

        groupHeaderArray[index] =
            {
                startColumnName:'GOLD_COUNT_' + log_id,
                numberOfColumns:4,
                titleText:COIN_LOG_INFO_ARRAY[log_id]
            };
        index++;
    }

    $("#item_table").jqGrid('setGroupHeaders', {
        useColSpanStyle:true,
        groupHeaders:groupHeaderArray
    });

    $("#item_table").jqGrid('setFrozenColumns');
    $("#t_item_table")
        //.append('<input type="button" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    //$("input","#t_item_table").click(function() { clickChartButton('아이템 유료 결제수', '#item_table'); });
    $("#click_excel","#t_item_table").click(function() { clickExcelButton(); });
}

$(function() {
    $('#chart-viewer').dialog({
        autoOpen:false,
        width:1000,
        height:700,
        modal:true,
        resizable:false,
        buttons:{
            "Close": function() {
                $(this).dialog('close');
            }
        },
        open:function() {
        },
        close:function() {
        }
    });
});
</script>

<div class="well">
  Period :
  <input type="text" name="start_date" id="start_date" class="date" size="8" readonly="readonly" />
  ~
  <input type="text" name="end_date" id="end_date" class="date" size="8" readonly="readonly" />

  <ul class="nav nav-pills">
    <li><a href="javascript:;" onclick="showCoinOfLevel();">Coin average by level</a></li>
    <li><a href="javascript:;" onclick="showRuneCountGrid();">Rune Inventory</a></li>
    <li><a href="javascript:;" onclick="showLotboxCountGrid();">Lottery box</a></li>
    <li><a href="javascript:;" onclick="showCoinGainCountGrid();">Daily gainned coin amout</a></li>
    <li><a href="javascript:;" onclick="showCoinUseCountGrid();">Daily used coin amount</a></li>
  </ul>

  <div id="grid-area"></div>
</div>

<div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
  <div id="chart_div"></div>
</div>