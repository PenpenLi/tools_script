<script type="text/javascript">
google.load("visualization", "1", {packages:["corechart"]});

var COUNTRY_NAME_ARRAY = <?php echo json_encode($COUNTRY_NAME_ARRAY); ?>;

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

function showUserJoinedCountOfCountry() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 0; index < COUNTRY_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = COUNTRY_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], index:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'country', 'ajaxGetUserJoinedCountOfCountryData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User joined count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showActiveUserCountOfCountry() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 0; index < COUNTRY_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = COUNTRY_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], index:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'country', 'ajaxGetActiveUserCountOfCountryData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"Active user count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showRemainedUserCountOfCountry() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Total'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:90, align:"right", sortable:false, frozen:true}
        ];

    for (var i = columns_name.length, index = 0; index < COUNTRY_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = COUNTRY_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], index:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], width:70, align:"center", sortable:false};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'country', 'ajaxGetRemainedUserCountOfCountryData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"Remained user count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showUserConnectedCountOfCountry() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Sum'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:70, align:"right", sortable:false, formatter:'integer', frozen:true}
        ];

    for (var i = columns_name.length, index = 0; index < COUNTRY_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = COUNTRY_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], index:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], width:70, align:"right", sortable:false, formatter:'integer'};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'country', 'ajaxGetUserConnectedCountOfCountryData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User connected count"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
}

function showUserPlayTimeOfCountry() {
    $('#grid-area').empty();

    var columns_name = ['Date', 'Total'];
    var columns_detail = [
            {name:'STAT_DATE', index:'STAT_DATE', width:90, align:"center", sortable:false, frozen:true},
            {name:'TOTAL_COUNT', index:'TOTAL_COUNT', width:100, align:"right", sortable:false, frozen:true}
        ];

    for (var i = columns_name.length, index = 0; index < COUNTRY_NAME_ARRAY.length; i++, index++) {
        columns_name[i] = COUNTRY_NAME_ARRAY[index];
        columns_detail[i] = {name:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], index:'USER_COUNT_' + COUNTRY_NAME_ARRAY[index], width:80, align:"center", sortable:false};
    }

    $('<table id="count_table"></table>').appendTo($('#grid-area')).jqGrid({
        url:'<?php echo site_url(array('log', 'country', 'ajaxGetUserPlayTimeOfCountryData')); ?>',
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:columns_name,
        colModel:columns_detail,
        rowNum:-1,
        multiselect:true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        jsonReader:{
            repeatitems:false,
            root:"rows"
        },
        width:1000,
        height:"auto",
        //toolbar:[true,"top"],
        caption:"User play time (minutes)"
    });

    $("#count_table").jqGrid('setFrozenColumns');
    $("input","#t_count_table").click(function() { clickChartButton('User Count by Level', '#count_table'); });
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
    <li><a href="javascript:;" onclick="showUserJoinedCountOfCountry();">User joined count</a></li>
    <li><a href="javascript:;" onclick="showActiveUserCountOfCountry();">Active user count</a></li>
    <li><a href="javascript:;" onclick="showRemainedUserCountOfCountry();">Remained user count</a></li>
    <li><a href="javascript:;" onclick="showUserConnectedCountOfCountry();">User connected count</a></li>
    <li><a href="javascript:;" onclick="showUserPlayTimeOfCountry();">User play time</a></li>
  </ul>

  <div id="grid-area"></div>
</div>

<div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
  <div id="chart_div"></div>
</div>