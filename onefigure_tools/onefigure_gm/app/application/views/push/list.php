<script type="text/javascript">
$(function() {
    $('#grid_table').jqGrid({
        url:'<?php echo site_url(array('push', 'ajaxGetMessage')); ?>',
        editurl:'<?php echo site_url(array('push', 'ajaxSetMessage')); ?>',
        mtype:'POST',
        datatype:'json',
        colNames:['No', 'Title', 'Message', 'Send Date', 'Send Hour', 'OS', 'OS', 'USER_CODE', 'Finish Date', 'Recieved Users', 'Created Date'],
        //colNames:['No', 'Title', 'Message', 'Send Date', 'Send Hour', 'OS', 'OS', 'Finish Date', 'Recieved Users', 'Created Date'],
        colModel:[
            {name:'MESSAGE_ID', index:'MESSAGE_ID', width:50, align:'center'},
            {name:'TITLE', index:'TITLE', width:150, editable:true, editrules:{required:true}, editoptions:{size:50,maxlength:100}},
            {name:'MESSAGE', index:'MESSAGE', width:250, editable:true, editrules:{required:true}, editoptions:{size:50,maxlength:100}},
            {name:'SEND_DATE', index:'SEND_DATE', width:100, align:'center', editable:true, editrules:{required:true}, editoptions:{defaultValue:'<?php echo date('Y-m-d'); ?>'}},
            {name:'SEND_TIME', index:'SEND_TIME', width:80, align:'center', editable:true, editrules:{required:true}, editoptions:{defaultValue:'<?php echo date('H', strtotime('+1 hour')); ?>'}},
            {name:'OS_TYPE', index:'OS_TYPE', width:80, align:'center', hidden:true, editable:true, editrules:{edithidden:true, required:true}, edittype:'select', editoptions:{name:'OS_TYPE', value:'3:All;2:Andriod;1:iOS'}},
            {name:'OS_NAME', index:'OS_TYPE', width:80, align:'center'},
            {name:'USER_CODE', index:'USER_CODE', width:80, align:'center', hidden:true, editable:true, editrules:{edithidden:true, required:true}, edittype:'select', editoptions:{name:'USER_CODE', value:'3:Tangent;1:Purple;0:USER'}},
	    {name:'FINISH_DATE', index:'FINISH_DATE', width:130, align:'center'},
            {name:'SUCCESS_COUNT', index:'SUCCESS_COUNT', width:100, align:'center', formatter:'integer'},
            {name:'CREATE_DATE', index:'CREATE_DATE', width:130, align:'center'}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager:'#pager',
        jsonReader:{
            repeatitems:false,
            root:"rows",
            id:'MESSAGE_ID'
        },
        viewrecords:true,
        sortorder:'desc',
        height:'100%',
        caption:'Push Messages'
    });

    $('#grid_table').jqGrid(
        'navGrid',
        '#pager',
        {edit:true, add:true, del:false},
        {width:500, height:250, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true}, // edit options
        {width:500, height:250, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true, closeAfterAdd:true}, // add options
        {reloadAfterSubmit:false, jqModal:true, closeOnEscape:true} // del options
    );
});
</script>

<div id="grid">
<table id="grid_table"></table>
<div id="pager"></div>
</div>
