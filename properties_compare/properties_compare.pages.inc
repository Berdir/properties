<?php
// $Id$

/**
 * @file
 * User page callbacks for properties_compare module.
 */

/**
 * Page callback, display comparison table.
 */
function properties_compare_page() {
  drupal_set_title(t('Comparison'));

  // Load current compare items.
  $list = properties_compare_list();
  if (empty($list)) {
    return t('Nothing to compare.');
  }

  $categories = array();
  $attributes = array();
  // First, loop over compare items to collect categories and attributes.
  foreach ($list as $item) {
    // Load fields attached to the entity.
    $fields = field_info_instances($item['entity_type'], field_extract_bundle($item['entity_type'], $item['entity']));

    // Get the display language for the entity.
    $languages = field_language($item['entity_type'], $item['entity']);

    // Loop over fields.
    foreach ($fields as $field_name => $field) {
      $field_info = field_info_field_by_id($field['field_id']);

      // Only handle properties fields.
      if ($field_info['type'] == 'properties') {
        // Load content for the configured language.
        $field_all_languages = $item['entity']->$field_name;
        $field_content = $field_all_languages[$languages[$field_name]];

        // Loop over content of that field.
        foreach ($field_content as $property) {
          $id = properties_compare_create_key($item['entity_type'], $item['entity']);

          // Collect number of entities that have a given category.
          $categories[$property['category']][$id] = TRUE;

          // Keep values in a hierarchic way for the next step.
          $attributes[$property['category']][$property['attribute']][$id] = $property['value'];
        }
      }
    }
  }
  // Remove categories which are only used once.
  $shared_categories = array_filter($categories, 'properties_compare_filter_categories');
  if (empty($shared_categories)) {
    return t('Nothing to compare.');
  }
  // Load categories.
  $loaded_categories = properties_category_load_multiple(array_keys($shared_categories));

  // Collect header.
  $header = array(t('Attributes'));
  foreach ($list as $item) {
    $header[] = entity_label($item['entity_type'], $item['entity']);
  }

  // Collect rows.
  $rows = array();
  foreach ($shared_categories as $shared_category => $value) {
    // Add a header row for the category.
    $rows[] = array(
      array(
        'data' => $loaded_categories[$shared_category]->label,
        'header' => TRUE,
        'colspan' => count($list) + 1,
      ),
    );

    // Load attributes for this category.
    $loaded_attributes = properties_attribute_load_multiple(array_keys($attributes[$shared_category]));

    // Loop over attributes of this category.
    foreach ($attributes[$shared_category] as $attribute_name => $field_values) {
      // First cell is the attribute label.
      $row = array(array('data' => $loaded_attributes[$attribute_name]->label));
      // Loop over entites in list.
      foreach ($list as $item) {
        $id = properties_compare_create_key($item['entity_type'], $item['entity']);
        // Check if this entity has a value for this attribute. If yes, display
        // the value, if not, display as an empty cell.
        if (isset($field_values[$id])) {
          $row[] = $field_values[$id];
        }
        else {
          $row[] = '&nbsp;';
        }
      }
      $rows[] = $row;
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}