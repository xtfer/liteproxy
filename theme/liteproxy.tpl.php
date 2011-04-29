<?php
/**
 * Template for rendering anything
 */
 
?>
<div class="liteproxy-proxied">
  <?php
  foreach($elements as $element){ ?>
    <div class="liteproxy-item">
      <?php if(isset($element['label'])){ ?>
        <span class="label"><?php echo $element['label']; ?></span>
      <?php }?>
      <?php echo $element['value']; ?>
    </div>
    
  <?php } ?>