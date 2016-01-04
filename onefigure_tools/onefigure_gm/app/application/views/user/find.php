<div class="row small">
    <div class="col-md-2"></div>
    <div class="col-md-8">
        <table class="table table-striped table-bordered">
            <colgroup>
                <col width="100" />
                <col width="" />
                <col width="150" />
                <col width="100" />
                <col width="150" />
            </colgroup>

            <thead>
            <tr>
                <th class="text-center">User ID</th>
                <th class="text-center">Platform ID</th>
                <th class="text-center">Nickname</th>
                <th class="text-center">Level</th>
                <th class="text-center">Created</th>
            </tr>
            </thead>

            <tbody id="data-body">
            <?php
            foreach ($USER_ROWS as $row) {
            ?>
            <tr>
                <td class="text-center"><?php echo $row['USER_ID']; ?></td>
                <td class="text-center"><?php echo $row['EMAIL']; ?></td>
                <td class="text-center"><a href="<?php echo site_url(array('user', 'view', $row['USER_ID'])); ?>"><?php echo $row['NICKNAME']; ?></a></td>
                <td class="text-center"><?php echo $row['LEVEL']; ?></td>
                <td class="text-center"><?php echo $row['CREATE_DATE']; ?></td>
            </tr>
            <?php
            }
            ?>
            </tbody>
        </table>
    </div>
</div>