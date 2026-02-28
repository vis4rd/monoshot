#pragma once

/**
 * @brief Disallow calling a member function on a temporary object.
 */
#define MONO_DISALLOW_CALL_ON_TEMP &

/**
 * @brief Allow calling a member function on a temporary object only.
 */
#define MONO_DISALLOW_CALL_ON_NONTEMP &&
