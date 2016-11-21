<script type="text/javascript">
    $(function() {
        $('#data_form .btn-danger').click(function() {
            $.ajax({
                "url":"<?php echo site_url(array('data', 'ajaxRefreshDocs')); ?>",
                "data":$('#data_form').serialize(),
                "type":"POST",
                "dataType":"text",
                "success":function(response) {
                    alert(response);
                }
            });

            return false;
        });

        $('#data_form .btn-warning').click(function() {
            $.ajax({
                "url":"<?php echo site_url(array('data', 'ajaxRefreshCache')); ?>",
                "data":$('#data_form').serialize(),
                "type":"POST",
                "dataType":"text",
                "success":function(response) {
                    alert(response);
                }
            });

            return false;
        });
    });
</script>

<div class="row small">
    <div class="col-md-6">
        <div class="panel panel-default">
            <div class="panel-body">
                <form method="post" id="data_form" role="form">
                    <div class="form-group">
                        <label for="exampleInputEmail1">Hosts</label>
                        <textarea name="hosts" class="form-control" rows="10"><?php echo $HOSTS; ?></textarea>
                    </div>
                    <button type="submit" class="btn btn-danger">1. Docs Refresh</button>
                    <button type="submit" class="btn btn-warning">2. Cache Refresh</button>
                </form>
            </div>
        </div>
    </div>

    <div class="col-md-6">
    </div>
</div>