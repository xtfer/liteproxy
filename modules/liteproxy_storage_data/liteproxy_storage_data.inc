<?php
/**
 * Utilities for creating and accesing tables using the Data module
 */

function liteproxy_storage_data_load(){
  
} 

function liteproxy_storage_data_save(){
  
}

function liteproxy_storage_data_delete(){
  
}

/**
 * Access a data table
 */

function liteproxy_load_data_table($table_name){
  
  // Sanity check: check Data module exists
  if(!module_exists('data')){
    drupal_set_message('The Data module was not found. Please install this to keep using Node Data Sync.');
    return FALSE;
  }
  
  // load a data table, and create it if it doesnt exist
  if(data_get_table($table_name)) {
    $data = data_get_handler($table_name);

    if(empty($data)){
      _liteproxy_init_data_table($table_name);
    } else {
      return $data;
    }
  } else {
    _liteproxy_init_data_table($table_name);
  }
  
  return FALSE;
}

/**
 * Create a new data table if one does not exist
 */
function _liteproxy_init_data_table(){
  
  // See if our table exists
  $table = data_get_table('liteproxy');

  // If this table is not available create a table.
  if (!$table) {
    $schema = 
    
    if($table = data_create_table(NDS_DATA_TABLE_MAPPINGS, $schema)){
      drupal_set_message('Created the table '.NDS_DATA_TABLE_MAPPINGS);
    } else {
      drupal_set_message('Could not create Data table!');
    }

  }
  
  return $table;
}

/**
 * Wrapper for the Data table handler
 */
function liteproxy_write_record($table, $data){
  $table = data_get_table($table);
  $table->handler()->save($data);
}