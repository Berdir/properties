<?php

$element = $variables['element'];
$output = '';

$table_id = 'attributes-values';
$order_class = 'attributes-delta-order';
$required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required. ') . '">*</span>' : '';

$header = array(
  array(
    'data' => '<label>' . t('!title: !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
    'colspan' => 2,
    'class' => array('field-label'),
  ),
  t('Label'),
  t('Order'),
);
$rows = array();

// Sort items according to '_weight' (needed when the form comes back after
// preview or failed validation)
$items = array();
foreach (element_children($element) as $key) {
  if (is_int($key)) {
    $items[] = &$element[$key];
  }
}
usort($items, '_field_sort_items_value_helper');

// Add the items as table rows.
foreach ($items as $key => $item) {
  $item['_weight']['#attributes']['class'] = array($order_class);
  $delta_element = drupal_render($item['_weight']);

  $label = drupal_render($item['label']);
  $cells = array(
    array('data' => '', 'class' => array('field-multiple-drag')),
    array('data' => drupal_render($item), 'class' => array('properties-attribute-row-small')),
    $label,
    array('data' => $delta_element, 'class' => array('delta-order')),
  );
  $rows[] = array(
    'data' => $cells,
    'class' => array('draggable'),
  );
}

$output = '<div class="form-item">';
$output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-multiple-table'))));
$output .= $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '';
$output .= '<div class="clearfix">' . drupal_render($element['new_category']) . drupal_render($element['add_more']) . '</div>';
$output .= '</div>';

drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);

echo $output;
