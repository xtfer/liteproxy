<?php
/**
 * liteproxy class
 */

class Liteproxy {
  
  public $name;
  
  /**
   * Configuration variables, built at object instantation
   */
  public $storage;
  public $mapping = array();
  public $request = array();
  public $display = array();
  
  /**
   * Activity variables, built during a retrieve process
   */
  public $content = array();
  
  public function __construct($node){
    // find the correct proxy
    $this->match_proxy();
    // define basic configuration
    $this->define($node);
    // load the storage engine
    $this->load_storage();
  }
  
  /**
   * Definition function
   */
  private function define(){
    // storage API: default is Cache, but others can be defined
    $this->storage = array(
      'type' => 'cache'
      );
      
    // mapping
    $this->mapping = array(
      // content type to proxy into
      'content_type' => '#content_type',
      // field containing the identifier of the object to retrieve
      'field' => '#field_name',
      // function to retrieve the field value with
      'field_processor' => $this->field_value(),
      );
      
    // request parameters
    $this->request = array(
      // basic url identifier to retrieve the object
      'base_url' => '',
      // additional path information
      'path' => '',
      // data type - this will be the extension on the file returned, if any
      'data_type' => '',
      // pass the basic values into a prefetch function to return values for the fetcher
      'prefetch' => $this->prefetch(),
      );
    
    // display
    $this->display = array(
      // process data retrieved from the source
      'prerender' => $this->prerender(),
      // theme function to use for styling
      'render' => $this->render(),
      // flag to replace the body with the rendered content
      'override_body' => 'FALSE'
      );
  }
  
  /**
   * Main CRUD functions
   */
  /**
   * Called when a node is viewed.
   * - Checks if a local copy exists
   * - Checks if it has expired
   * - Performs a retrieve operation
   * - Runs save
   * - prepares the content for viewing
   */
  public function view($record_id){
    $data = $this->storage['engine']->read($record_id);
    
    
    
  }
  
  public function save($record_id, $data){
    return $this->storage['engine']->update($record_id, $data);
  }
  
  public function delete($record_id){
    return $this->storage['engine']->delete($record_id);
  }
  
  /**
   * Utilities
   */
  private function load_storage(){  
    $storages = liteproxy_storage_list();
    if(in_array($this->storage['type'], $storages)){
      $handler = $storages[$storage['type']]['handler'];
      $this->storage['engine'] = new $handler;
    }
  }
  
  /**
   * Activity functions
   */
  public function field_value($node){
    $field = $this->mapping->field;
    $this->request->id = $node->$field[0]['value'];
  }
  
  public function prefetch(){
    $this->request->url = $this->request->base_url.'/'.$this->request->path.'/'.$this->request->id.'.'.$this->request->type; 
  }
  
  public function fetch(){
    $success_codes = array(200, 202, 301, 302, 303, 304, 305, 307);

    $response = drupal_http_request($this->request->url);
    
    if (in_array($result->code, $success_codes)) {
      $this->content['raw'] = $response->data;
    }
    else {
      watchdog('liteproxy', 'Failed to fetch remote data. <br /> <strong>Request</strong><br /> @request <br /> <strong>Error</strong><br /> @error', array('@request' => $request, '@error' => $response->error.'('. $response->code.')'));
      $this->content['raw'] = FALSE;
    }
  }
  
  public function prerender(){
    $elements = array();
    foreach($this->content['raw'] as $key => $value) {
      $elements[$key]['label'] = check_plain($key);
      $elements[$key]['value'] = check_markup($value, FILTER_FORMAT_DEFAULT, FALSE);
    }
    $this->content['processed'] = $elements;
  }
  
  public function render(){
    $this->content['rendered'] = theme('liteproxy', $this->content['processed']);
  }
  
  /**
   * Standard method for processing a liteproxy request
   */ 
  function retrieve(&$node){
    
    // Fetch the content
    $this->fetch();

    // process the returned content
    $this->prerender();

    // render the content for output
    $this->render();

    // Set values on the node
    $node['liteproxy']['content'] = $this->content;
    if(isset($this->display['override_body']) && $this->display['override_body'] == TRUE){
      $node['body'] = $this->content;
    }
  }
  
  /**
   * Match a proxy
   * @param $node a node object
   * @return a proxy configuration array, or FALSE
   */
  public function match_proxy($node){
    $proxies = $this->get_proxies();

    foreach($proxies as $proxy){
      if($proxy['content_type'] == $node->type) {
        return $proxy;
      }
    }

    return FALSE;
  }

  /**
   * Return a list of proxies.
   * Statically cached.
   * @return an array of proxies, or FALSE if no proxies are found.
   */
  public function get_proxies(){
    static $proxies = array();

    if(!isarray($proxies) || empty($proxies)){
      $proxies = module_invoke_all('liteproxy_proxies');
    }

    if(!isarray($proxies) || empty($proxies)){
      return FALSE;
    }

    return $proxies;
  }
}

/**
 * Storage definition
 */
abstract class liteproxyStorage {

  abstract public function read($record_id);
  
  abstract public function update($record_id);
  
  abstract public function delete($record_id);
}