<div class="row small">
    <div class="col-md-2"></div>
    <div class="col-md-8">
        <div class="panel panel-default">
            <div class="panel-heading">
                <a href="<?php echo site_url(array('notice', 'edit')); ?>" class="btn btn-primary">Write</a>
            </div>

            <table class="table table-bordered table-responsive">
                <colgroup>
                    <col width="60" />
                    <col />
                    <col width="200" />
                    <col width="60" />
                </colgroup>

                <thead>
                <tr>
                    <th class="text-center">ID</th>
                    <th class="text-center">Title</th>
                    <th class="text-center">Period of activated</th>
                    <th class="text-center">Visible</th>
                </tr>
                </thead>

                <tbody>
                <?php
                foreach ($NOTICE_ROWS as $row) {
                ?>
                <tr>
                    <td class="text-center"><?php echo $row['NOTICE_ID']; ?></td>
                    <td><a href="<?php echo site_url(array('notice', 'edit', $row['NOTICE_ID'])); ?>"><?php echo $row['TITLE']; ?></a></td>
                    <td class="text-center"><?php echo date('Y-m-d', strtotime($row['START_DATE'])); ?> ~ <?php echo date('Y-m-d', strtotime($row['END_DATE'])); ?></td>
                    <td class="text-center"><?php echo ($row['IS_SHOW'] == 1) ? 'Show' : 'Hide'; ?></td>
                </tr>
                <?php } ?>
                </tbody>
            </table>
        </div>
    </div>
</div>