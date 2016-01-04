<script type="text/javascript">
google.load("visualization", "1", {packages:["corechart"]});

var GRID_ARRAY = {
         url:"",
         mtype: "POST",
         postData:{},
         datatype: "json",
         colNames:['Date', 'Sum', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23'],
         colModel:[
            {name:'date', index:'date', width:90, align:"center", frozen:true},
            {name:'total', index:'total', width:80, align:"right", formatter:'integer', frozen:true},
            {name:'hour0', index:'hour0', width:70, align:"right", formatter:'integer'},
            {name:'hour1', index:'hour1', width:70, align:"right", formatter:'integer'},
            {name:'hour2', index:'hour2', width:70, align:"right", formatter:'integer'},
            {name:'hour3', index:'hour3', width:70, align:"right", formatter:'integer'},
            {name:'hour4', index:'hour4', width:70, align:"right", formatter:'integer'},
            {name:'hour5', index:'hour5', width:70, align:"right", formatter:'integer'},
            {name:'hour6', index:'hour6', width:70, align:"right", formatter:'integer'},
            {name:'hour7', index:'hour7', width:70, align:"right", formatter:'integer'},
            {name:'hour8', index:'hour8', width:70, align:"right", formatter:'integer'},
            {name:'hour9', index:'hour9', width:70, align:"right", formatter:'integer'},
            {name:'hour10', index:'hour10', width:70, align:"right", formatter:'integer'},
            {name:'hour11', index:'hour11', width:70, align:"right", formatter:'integer'},
            {name:'hour12', index:'hour12', width:70, align:"right", formatter:'integer'},
            {name:'hour13', index:'hour13', width:70, align:"right", formatter:'integer'},
            {name:'hour14', index:'hour14', width:70, align:"right", formatter:'integer'},
            {name:'hour15', index:'hour15', width:70, align:"right", formatter:'integer'},
            {name:'hour16', index:'hour16', width:70, align:"right", formatter:'integer'},
            {name:'hour17', index:'hour17', width:70, align:"right", formatter:'integer'},
            {name:'hour18', index:'hour18', width:70, align:"right", formatter:'integer'},
            {name:'hour19', index:'hour19', width:70, align:"right", formatter:'integer'},
            {name:'hour20', index:'hour20', width:70, align:"right", formatter:'integer'},
            {name:'hour21', index:'hour21', width:70, align:"right", formatter:'integer'},
            {name:'hour22', index:'hour22', width:70, align:"right", formatter:'integer'},
            {name:'hour23', index:'hour23', width:70, align:"right", formatter:'integer'}
        ],
        rowNum:-1,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        width:1000,
        height:"auto",
        toolbar:[true,"top"],
        caption:""
    };

function showGrid() {
    $('#grid-area').empty().append($('<table id="grid_table"></table>'));
    $('#grid_table').jqGrid({
        url:"<?php echo site_url(array('stats', 'ajaxSelectUserStat')); ?>",
        mtype: "POST",
        postData:{ 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } },
        datatype: "json",
        colNames:['날짜', '누적 회원수', '가입자수', '접속 회원수'],
        colModel:[
            {name:'date', index:'date', width:90, align:"center", frozen:true},
            {name:'user_count', index:'user_count', width:80, align:"right", formatter:'integer'},
            {name:'join_count', index:'join_count', width:80, align:"right", formatter:'integer'},
            {name:'login_count', index:'login_count', width:80, align:"right", formatter:'integer'}
        ],
        rowNum:-1,
        multiselect: true,
        shrinkToFit:false,
        viewrecords:true,
        sortorder:"desc",
        width:"100%",
        height:"auto",
        toolbar:[false,"top"],
        caption:"회원 통계"
    });
    $('#grid_table').jqGrid('setFrozenColumns');
    $('#t_grid_table')
        .append('<input type="button" id="click_graph" value="Open Graph Choose Rows"/>')
        .append('<input type="button" id="click_excel" value="Download Excel"/>');
    $("#click_graph","#t_grid_table").click(function() { clickChartButton(title, 'grid_table'); });
    $("#click_excel","#t_grid_table").click(function() { clickExcelButton(excel_url); });
}

function drawChart(title, data_array) {
    var data = google.visualization.arrayToDataTable(data_array);

    var options = {
        'title':title,
        'width':950,
        'height':550,
        'fontSize':11,
        'chartArea':{left:60,top:20,width:800,height:500}
    };

    var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
    chart.draw(data, options);
}

function clickChartButton(title, grid_id) {
    var selected_rows_id = $("#" + grid_id).jqGrid('getGridParam', 'selarrrow');
    var selected_rows = null;

    selected_rows_id.sort();

    if (selected_rows_id.length == 0) {
        alert('Please, Choice Data.');
        return false;
    }

    var data_array = [['Hour']];
    var date_index = 1;

    for (var i=0; i<24; i++) {
        data_array[(i + 1)] = [i + 'H'];
    }

    for (var index in selected_rows_id) {
        selected_rows = $("#" + grid_id).jqGrid('getRowData', selected_rows_id[index]);
        data_array[0][date_index] = selected_rows_id[index];

        for (var i=0; i<24; i++) {
            data_array[i + 1][date_index] = parseInt(selected_rows['hour' + i]);
        }

        date_index++;
    }

    drawChart(title, data_array);
    $('#chart-viewer').dialog('open');
}

function clickExcelButton(url) {
    location.href = url;
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

<div class="row small">
    <div class="col-sm-2">
        <ul class="nav nav-pills nav-stacked">
            <li role="presentation"><a href="javascript:;" onclick="showGrid();">기본 정보</a></li>
        </ul>
    </div>
    <div class="col-sm-10">
        <div class="panel panel-default">
            <div class="panel-heading">
                <form class="form-inline">
                    <div class="form-group">
                        검색기간 :
                        <input type="text" name="start_date" id="start_date" class="date form-control" size="8" readonly="readonly" />
                        ~
                        <input type="text" name="end_date" id="end_date" class="date form-control" size="8" readonly="readonly" />
                    </div>
                </form>
            </div>
            <div class="panel-body">
                <div id="grid-area"></div>
            </div>
        </div>

        <div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
            <div id="chart_div"></div>
        </div>
    </div>
</div>