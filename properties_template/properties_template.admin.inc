<?php
// $Id$

/**
 * @file
 * Contains admin menu callbacks for properties_template.module.
 */

/**
 * Menu callback; Display a list of templates
 */
function properties_template_admin_templates_list() {
  $templates = properties_template_load_paging(20);

  $header = array(t('Name'), t('Label'), t('Operations'));
  $rows = array();
  $url_options = array('query' => drupal_get_destination());
  foreach ($templates as $template) {
    $links = array(l('edit', 'admin/config/content/properties/templates/edit/' . $template->name, $url_options), l('delete', 'admin/config/content/properties/templates/delete/' . $template->name, $url_options));

    $rows[] = array(
      $template->name,
      check_plain($template->label),
      implode(' ', $links),
    );
  }
  return theme('table', array('rows' => $rows, 'header' => $header, 'empty' => t('No templates found.')));
}

/**
 * Form builder; Add/Edit form for templates.
 */
function properties_template_admin_templates_form($form, &$form_state, $template = NULL) {
  if (!isset($form_state['template'])) {
    if (empty($template)) {
      $template = (object) array(
        'label' => '',
        'name' => '',
        'categories' => array(),
        'new' => TRUE,
      );
    }
    $form_state['template'] = $template;
  }

  // Make sure there are always 3 empty categories displayed.
  $add_empty = true;
  foreach ($form_state['template']->categories as $category) {
    if (empty($category->name)) {
      $add_empty = false;
      break;
    }
  }
  if ($add_empty) {
    // Use a random id for these to trick out browsers.
    $form_state['template']->categories[] = (object)array(
      'name' => '',
      'label' => '',
      'weight' => count($form_state['template']->categories),
    );
  }

  drupal_add_css(drupal_get_path('module', 'properties') . '/properties_admin.css');

  $form['label'] = array(
    '#title' => t('Template label'),
    '#type' => 'textfield',
    '#default_value' => $form_state['template']->label,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $form_state['template']->name,
    '#maxlength' => 32,
    '#disabled' => empty($form_state['template']->new),
    '#machine_name' => array(
      'exists' => 'properties_template_load',
    ),
    '#description' => t('A unique machine-readable name for this category. It must only contain lowercase letters, numbers, and underscores. This name will be used when using this category.'),
  );

  $categories = array();
  $delta = 0;
  foreach ($form_state['template']->categories as $name => $category) {
    $element = array();
    $element['category'] = array(
      '#type' => 'textfield',
      '#default_value' => $category->name,
      '#autocomplete_path' => 'properties_autocomplete/category',
      '#ajax' => array(
        'callback' => 'properties_admin_update_label_js',
        'wrapper' => 'attributes-wrapper',
        'effect' => 'fade',
      ),
    );
    // TODO: add categories-wrapper

    // If name is given but label is empty, this means that this is a new
    // category. Display a textfield to enter a label.
    if (!empty($category->new)) {
      $element['label'] = array(
        '#type' => 'textfield',
        '#default_value' => $category->label,
        '#maxlength' => 255,
        '#required' => TRUE,
      );
    } else {
      $element['label'] = array(
        '#markup' => $category->label,
      );
    }

    // We name the element '_weight' to avoid clashing with elements
    // defined by widget.
    $element['_weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for row @number', array('@number' => $delta)),
      '#title_display' => 'invisible',
      '#default_value' => !empty($category->weight) ? $category->weight : $delta,
      '#weight' => 100,
    );
    $categories[!empty($category->name) ? $category->name : $delta] = $element;
    $delta++;
  }

  $categories += array(
    '#type' => 'fieldset',
    '#theme' => 'properties__template_admin_templates_categories_form',
    '#title' => t('Categories'),
    '#description' => t('Configure the default categories of this template.'),
    '#prefix' => '<div id="attributes-wrapper">',
    '#suffix' => '</div>',
    '#max_delta' => $delta,
    '#tree' => TRUE,
  );
  $categories['add_more'] = array(
    '#type' => 'submit',
    '#name' => 'categories_add_more',
    '#value' => t('Add another category'),
    '#categories' => array('class' => array('field-add-more-submit')),
    '#limit_validation_errors' => array(array('categories')),
    '#submit' => array('properties_template_admin_add_more_submit'),
    '#ajax' => array(
      'callback' => 'properties_template_admin_add_more_js',
      'wrapper' => 'attributes-wrapper',
      'effect' => 'fade',
    ),
  );

  // TODO: add categories-wrapper
  $form['categories'] = $categories;

  if (empty($template->name)) {
    $form['add'] = array(
      '#value' => t('Add'),
      '#type' => 'submit',
      '#submit' => array('properties_template_admin_templates_add'),
    );

    $form['add_another'] = array(
      '#value' => t('Add another'),
      '#type' => 'submit',
      '#submit' => array('properties_template_admin_templates_add_another'),
    );
  }
  else {
    $form['save'] = array(
      '#value' => t('Save'),
      '#type' => 'submit',
      '#submit' => array('properties_template_admin_templates_save'),
    );
  }
  return $form;
}
/**
 * Form builder; Valdate template form.
 */
function properties_template_admin_templates_form_validate($form, &$form_state) {
  // Update template object.
  $form_state['template']->label = $form_state['values']['label'];
  $form_state['template']->name = $form_state['values']['name'];
  $form_state['template']->categories = array();

  foreach ($form_state['values']['categories'] as $name => $category) {
    if (is_array($category) && isset($category['category'])) {
      if (empty($category['category'])) {
        $category_object = (object)array(
          'label' => '',
          'name' => '',
          'weight' => !empty($category['_weight']) ? $category['_weight'] : count($form_state['template']->categories),
        );
        $form_state['template']->categories[$name] = $category_object;
      }
      elseif ($category_object = properties_category_load($category['category'])) {
        $category_object->weight = $category['_weight'];
        $form_state['template']->categories[$category_object->name] = $category_object;
      }
      else {
        // Update empty category.
        $form_state['template']->categories[$category['category']]->name = $category['category'];
        $form_state['template']->categories[$category['category']]->label = isset($category['label']) ? $category['label'] : '';
        $form_state['template']->categories[$category['category']]->weight = $category['_weight'];
        $form_state['template']->categories[$category['category']]->new = TRUE;

        // If label is empty, add validation error.
        if (empty($form_state['template']->categories[$category['category']]->label)) {
          // Not possible to do this as a proper form validation step because
          // the form is not rebuilt in case of validation errors.
          drupal_set_message(t('Category %name does not exist, a label must be provided to create it.', array('%name' => $category['category'])), 'error');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
  }
}

/**
 * Ajax callback in response to a new empty widget being added to the form.
 *
 * This returns the new page content to replace the page content made obsolete
 * by the form submission.
 *
 * @see field_add_more_submit()
 */
function properties_template_admin_add_more_js($form, $form_state) {
  $element = $form['categories'];

  return $element;
}

/**
 * Form builder; Submit callback to save a template and redirect to list.
 */
function properties_template_admin_templates_add($form, &$form_state) {
  properties_template_save($form_state['template']);
  $form_state['redirect'] = 'admin/config/content/properties/templates';
  drupal_set_message(t('Template created.'));
}

/**
 * Form builder; Submit callback to save a template and redirect to list.
 */
function properties_template_admin_templates_save($form, &$form_state) {
  properties_template_save($form_state['template']);
  $form_state['redirect'] = 'admin/config/content/properties/templates';
  drupal_set_message(t('Template updated.'));
}

/**
 * Form builder; Submit callback to save a template and stay on form.
 */
function properties_template_admin_templates_add_another($form, &$form_state) {
  properties_template_save($form_state['template']);
  drupal_set_message(t('Template created.'));
}

/**
 * Submit handler for the "Add another item" button of a field form.
 *
 * This handler is run regardless of whether JS is enabled or not. It makes
 * changes to the form state. If the button was clicked with JS disabled, then
 * the page is reloaded with the complete rebuilt form. If the button was
 * clicked with JS enabled, then ajax_form_callback() calls field_add_more_js()
 * to return just the changed part of the form.
 */
function properties_template_admin_add_more_submit($form, &$form_state) {
  $attribute =
  $form_state['template']->categories[] = (object)array(
    'label' => '',
    'name' => '',
    'weight' => count($form_state['template']->categories),
  );
  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder function; Provide a confirmation form to delete a template.
 */
function properties_template_admin_templates_delete($form, &$form_state, $template) {
  $form['template'] = array(
    '#type' => 'value',
    '#value' => $template,
  );

  return confirm_form($form, t('Delete template %template', array('%template' => $template->label)), '');
}

/**
 * Submit handler to delete a template.
 */
function properties_template_admin_templates_delete_submit($form, &$form_state) {
  properties_template_delete($form_state['values']['template']);
  drupal_set_message(t('Template deleted.'));
}