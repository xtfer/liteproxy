<?php
/**
 * Basic caching mechanism for working with liteproxy content
 */

class liteproxyCacheStorage extends liteproxyStorage {
  
  /**
   * Get a cache record corresponding with the record id
   */
  public function read($record_id){
    return cache_get($record_id, 'cache_liteproxy');
  }
  
  /**
   * Set a cache record
   */
  public function update($record_id, $data){
    cache_set($record_id, $data, 'cache_liteproxy', CACHE_PERMANENT);
  }
  
  /**
   * Delete a cache record
   */
  public function delete($record_id){
    cache_clear_all($record_id, 'cache_liteproxy', FALSE);
  }
}