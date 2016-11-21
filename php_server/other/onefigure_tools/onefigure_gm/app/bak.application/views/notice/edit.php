<script type="text/javascript">
    $(function() {
        var dates = $('.period_date').datepicker({
            dateFormat:'yy-mm-dd',
            showOtherMonths: true,
            selectOtherMonths: true,
            onSelect: function(selectedDate) {
                var option = $(this).attr('name') == "start_date" ? "minDate" : "maxDate",
                    instance = $( this ).data( "datepicker" ),
                    date = $.datepicker.parseDate(
                        instance.settings.dateFormat ||
                        $.datepicker._defaults.dateFormat,
                        selectedDate, instance.settings );
                dates.not( this ).datepicker( "option", option, date );
            }
        });
    });
</script>

<div class="row small">
    <div class="col-md-2"></div>
    <div class="col-md-8">
        <div class="panel panel-default">
            <div class="panel-heading">Notice Edit</div>
            <form role="form" method="post" action="<?php echo site_url(array('notice', 'execSetNotice')); ?>">
                <input type="hidden" name="notice_id" value="<?php echo $NOTICE_ROW['NOTICE_ID']; ?>" />

                <div class="panel-body">
                    <div class="form-group">
                        <label for="form-title">Title</label>
                        <input type="text" id="form-title" name="title" class="form-control" value="<?php echo $NOTICE_ROW['TITLE']; ?>" placeholder="Title" required="required" />
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-start-date">Period</label>
                        <input type="date" id="form-start-date" name="start_date" class="form-control period_date" value="<?php echo date('Y-m-d', strtotime(isset($NOTICE_ROW['START_DATE']) ? $NOTICE_ROW['START_DATE'] : 'today')); ?>" placeholder="Start" required="required" />
                        ~
                        <input type="date" id="form-end-date" name="end_date" class="form-control period_date" value="<?php echo date('Y-m-d', strtotime(isset($NOTICE_ROW['END_DATE']) ? $NOTICE_ROW['END_DATE'] : '+5 days')); ?>" placeholder="End" required="required" />
                    </div>
                    <div class="form-group form-inline">
                        <label for="form-visible">Visible</label>
                        <input type="radio" name="is_show" class="form-control" value="1" <?php echo (isset($NOTICE_ROW['IS_SHOW']) && $NOTICE_ROW['IS_SHOW'] == 1) ? 'checked="checked"' : null; ?> /> Show
                        <input type="radio" name="is_show" class="form-control" value="0" <?php echo (isset($NOTICE_ROW['IS_SHOW']) && $NOTICE_ROW['IS_SHOW'] == 0) ? 'checked="checked"' : null; ?> /> Hide
                    </div>
                    <div class="form-group">
                        <label for="form-content-english">Content</label>
                        <div role="panel">
                            <ul class="nav nav-tabs" role="tablist">
                                <li role="presentation" class="active"><a href="#english" aria-controls="english" role="tab" data-toggle="tab">English</a></li>
                                <li role="presentation"><a href="#korean" aria-controls="korean" role="tab" data-toggle="tab">Korean</a></li>
                                <li role="presentation"><a href="#japanese" aria-controls="japanese" role="tab" data-toggle="tab">Japanese</a></li>
                                <li role="presentation"><a href="#chinese" aria-controls="chinese" role="tab" data-toggle="tab">Chinese</a></li>
                            </ul>

                            <div class="tab-content">
                                <div role="tabpanel" class="tab-pane active" id="english"><textarea id="form-content-english" name="content[English]" class="form-control" rows="10"><?php echo isset($NOTICE_ROW['CONTENT']['English']) ? $NOTICE_ROW['CONTENT']['English'] : null; ?></textarea></div>
                                <div role="tabpanel" class="tab-pane" id="korean"><textarea name="content[Korean]" class="form-control" rows="10"><?php echo isset($NOTICE_ROW['CONTENT']['Korean']) ? $NOTICE_ROW['CONTENT']['Korean'] : null; ?></textarea></div>
                                <div role="tabpanel" class="tab-pane" id="japanese"><textarea name="content[Japanese]" class="form-control" rows="10"><?php echo isset($NOTICE_ROW['CONTENT']['Japanese']) ? $NOTICE_ROW['CONTENT']['Japanese'] : null; ?></textarea></div>
                                <div role="tabpanel" class="tab-pane" id="chinese"><textarea name="content[Chinese]" class="form-control" rows="10"><?php echo isset($NOTICE_ROW['CONTENT']['Chinese']) ? $NOTICE_ROW['CONTENT']['Chinese'] : null; ?></textarea></div>
                            </div>
                        </div>
                    </div>
                </div>

            <div class="panel-footer">
                <button type="submit" class="btn btn-primary">Save</button>
                <button type="reset" class="btn btn-default">Cancel</button>
            </div>
            </form>
        </div>
    </div>
</div>