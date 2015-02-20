/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOSETTINGS_H
#define PHVIDEOSETTINGS_H

/**
 * @brief The settings for the PhVideoEngine
 */
class PhVideoSettings
{
public:
	/**
	 * @brief Screen delay compensation
	 * @return A value in millisecond
	 */
	virtual int screenDelay() = 0;

	/**
	 * @brief Display video in its native size
	 * @return True if native, false otherwise
	 */
	virtual bool useNativeVideoSize() = 0;
};

#endif // PHVIDEOSETTINGS_H
