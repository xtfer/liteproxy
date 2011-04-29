<?php
/**
 * Default liteproxy
 */

/**
 * Default implementation of a liteproxy definition
 *
 * This proxy configuration won't work on its own, but its functions work as fallbacks for 
 * other proxies. Some functions simply return NULL, as their value is essentially
 * undefined.
 * 
 * To define your own liteproxy, you can start by copying this file.
 *
 * @return
 *  an array of values defining the functions and properties set on the proxy
 */
function default_liteproxy_proxies(){
  $importer = array(
    'default' => array(
      
      // storage API: default is Data or Cache, but others can be defined
      'storage' => array(
        'type' => 'data',
        'schema' => array(),
        ),
          
      // mapping
      'mapping' => array(
        // content type to proxy into
        'content_type' => '#content_type',
        // field containing the identifier of the object to retrieve
        'field' => '#field_name',
        // function to retrieve the field value with
        'field_processor' => 'default_liteproxy_field_value',
        ),
        
      // request parameters
      'retrieval' => array(
        // basic url identifier to retrieve the object
        'base_url' => 'default_liteproxy_base_url',
        // additional path information
        'path' => 'default_liteproxy_path',
        // data type - this will be the extension on the file returned, if any
        'data_type' => 'xml',
        // pass the basic values into a prefetch function to return values for the fetcher
        'prefetch' => 'default_liteproxy_prefetch',
        // function to retrieve the object
        'fetcher' => 'default_liteproxy_fetch',
        ),
      
      // display
      'display' => array(
        // process data retrieved from the source
        'preprocess' => 'default_liteproxy_preprocess',
        // theme function to use for styling
        'theme' => 'default_liteproxy_render',
        // flag to replace the body with the rendered content
        'override_body' => 'FALSE'
        ),
        
      ),
    );
    
  return $importer;
}

/**
 * Implementation of HOOK_liteproxy_base_url()
 */
function default_liteproxy_base_url($node = array()){
  return NULL;
}

/**
 * Implementation of HOOK_liteproxy_path()
 */
function default_liteproxy_path($node = array()){
  return NULL;
}

/**
 * Implementation of HOOK_liteproxy_field_value()
 *
 * Return a value from a CCK field on the node for use in the request
 *
 * @param $node 
 *  the original node object
 * @param $field 
 *  a field name to retrieve a value from
 * @return 
 *  by default, the value from the first field
 */
function default_liteproxy_field_value($node, $field){
  return $node->$field[0]['value'];
}

/**
 * Implementation of HOOK_liteproxy_prefetch()
 *
 * This is an opportunity to alter the fetch object before sending it to the
 * fetcher.
 *
 * @param $request 
 *  a request object formed from the previous function calls
 * @return 
 *  an array of values for the fetcher to use. 
 */
function default_liteproxy_prefetch($request){ 
  return array(
    'request_url' => $request->base_url.'/'.$request->path.'/'.$request->id.'.'.$request->type, 
    'source' => $request
  );
}

/**
 * Implementation of HOOK_liteproxy_fetch()
 *
 * Make a request to the remote source for the object
 *
 * @param $request_vars
 *  The default fetcher expects
 *  - 'request_url': a fully-qualified URL to the object to be retrieved using
 * @return 
 *  the result of the request
 */
function default_liteproxy_fetch($request_vars){
  $response = drupal_http_request($request_vars['request_url']);
  
  // @todo error checking and logging
  // @todo modify the response format to pass only data back
  
  return $response;
}

/**
 * Implementation of HOOK_liteproxy_preprocess()
 *
 * @param $data
 *  the raw response data from the fetch
 * @return
 *  a processed data array ready for themeing.
 */
function default_liteproxy_preprocess($raw_data){
  $processed_data = NULL;
  
  $processed_data = $raw_data;
  
  return $processed_data;
}

/**
 * Implementation of HOOK_liteproxy_theme()
 *
 * @param $processed_data
 *  the processed data from HOOK_liteproxy_theme()
 * @return
 *  the fully themed output ready for display
 */
function default_liteproxy_theme($processed_data){
  return 'Data themed using the default liteproxy theme is undefined.';
}