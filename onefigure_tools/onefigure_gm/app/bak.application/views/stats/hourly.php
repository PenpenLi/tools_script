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

function showGrid(title, url) {
    GRID_ARRAY['caption'] = title;
    GRID_ARRAY['url'] = url;
    GRID_ARRAY['postData'] = { 'start_date':function() { return $('#start_date').val(); }, 'end_date':function() { return $('#end_date').val(); } };

    var excel_url = '<?php echo $this->uri->segment(1); ?>/' + url.replace('ajaxGetStatsHourlyData', 'downloadGetStatsHourlyData') + '/' + $('#start_date').val() + '/' + $('#end_date').val()

    $('#grid-area').empty().append($('<table id="grid_table"></table>'));
    $('#grid_table').jqGrid(GRID_ARRAY);
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

<div class="well">
  Period :
  <input type="text" name="start_date" id="start_date" class="date" size="8" readonly="readonly" />
  ~
  <input type="text" name="end_date" id="end_date" class="date" size="8" readonly="readonly" />

  <ul class="nav nav-pills">
    <li><a href="javascript:;" onclick="showGrid('Hourly joined count', '<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 1)); ?>');">Hourly joined count</a></li>
    <li><a href="javascript:;" onclick="showGrid('Hourly game play count', '<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 2)); ?>');">Hourly game play count</a></li>
    <li><a href="javascript:;" onclick="showGrid('Hourly connected count', '<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 3)); ?>');">Hourly connected count</a></li>
    <li><a href="javascript:;" onclick="showGrid('Hourly connected user count', '<?php echo site_url(array($this->uri->segment(1), 'stats', 'ajaxGetStatsHourlyData', 4)); ?>');">Hourly connected user count</a></li>
  </ul>

</div>
  <div id="grid-area"></div>

<div id="chart-viewer" title="Graph" style="display:none; margin:0px;">
  <div id="chart_div"></div>
</div>