<script type="text/javascript">
$(function() {
    $('#grid_table').jqGrid({
        url:'<?php echo site_url(array('manager', 'ajaxGetManager')); ?>',
        editurl:'<?php echo site_url(array('manager', 'ajaxSetManager')); ?>',
        mtype:'POST',
        datatype:'json',
        colNames:['No', 'Email', 'Password', 'Name', 'Level', 'Created Date', 'Connected Date'],
        colModel:[
            {name:'MANAGER_ID', index:'MANAGER_ID', width:50, align:'center'},
            {name:'EMAIL', index:'EMAIL', width:250, editable:true, editrules:{required:true}, editoptions:{size:50,maxlength:100}},
            {name:'PASSWORD', index:'PASSWORD', width:0, hidden:true, editable:true, editrules:{edithidden:true}, edittype:'password', editoptions:{size:20,maxlength:20}},
            {name:'NAME', index:'NAME', width:150, align:'center', editable:true, editrules:{required:true}},
            {name:'LEVEL', index:'LEVEL', width:60, align:'center', editable:true, editrules:{required:true}, edittype:'select', editoptions:{value:'0:0;1:1;2:2;3:3;4:4;5:5;6:6;7:7;8:8;9:9'}},
            {name:'CREATE_DATE', index:'CREATE_DATE', width:150, align:'center'},
            {name:'LOGIN_DATE', index:'LOGIN_DATE', width:150, align:'center'}
        ],
        rowNum:20,
        rowList:[20,50,100],
        pager:'#pager',
        jsonReader:{
            repeatitems:false,
            root:"rows",
            id:'MANAGER_ID'
        },
        viewrecords:true,
        sortorder:'desc',
        height:'100%',
        caption:'Managers'
    });

    $('#grid_table').jqGrid(
        'navGrid',
        '#pager',
        {edit:true, add:true, del:false},
        {width:500, height:290, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true}, // edit options
        {width:500, height:290, reloadAfterSubmit:false, jqModal:true, closeOnEscape:true, closeAfterAdd:true}, // add options
        {reloadAfterSubmit:false, jqModal:true, closeOnEscape:true} // del options
    );
});
</script>

<div id="grid">
<table id="grid_table"></table>
<div id="pager"></div>
</div>
