<div class="row">
  <div class="col-sm-9">
    <form method="post" action="<?php echo site_url(array('facebook', 'push', 'sendiOSPushMessage')); ?>" id="set-edit-form" class="form-horizontal" role="form">
      <div class="form-group">
        <label class="control-label col-sm-3" for="form-device-token">Device Token</label>
        <div class="col-sm-9">
          <textarea name="device_token" id="form-device-token" class="form-control" rows="10">%06%2A%73%AE%EB%F2%C3%2C%71%CF%89%6A%1E%61%97%79%D3%B2%25%58%94%2C%52%0E%B2%8A%43%41%E3%5F%A0%11
%B4%64%7A%88%57%84%62%42%42%22%D7%41%CA%49%16%53%89%E2%A1%3B%25%9F%02%C7%4F%07%46%0B%F2%28%D0%F1
          </textarea>
        </div>
      </div>
      <div class="form-group">
        <label class="control-label col-sm-3" for="form-message">Message</label>
        <div class="col-sm-9">
          <input type="text" name="message" id="form-message" class="form-control" value="Push Test" />
        </div>
      </div>
      <div class="form-group">
        <label class="control-label col-sm-3" for="form-service">Service</label>
        <div class="col-sm-9">
          <input type="radio" name="service" id="form-service" value="1" checked="checked" /> Product
          <input type="radio" name="service" id="form-service" value="0" /> Dev
        </div>
      </div>
      <div class="form-group">
        <div class="col-sm-9">
          <button type="submit" class="btn btn-default">전송</button>
        </div>
      </div>
    </form>
  </div>
</div>