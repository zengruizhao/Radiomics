#ifndef __ccipdFilteringGroups_h
#define __ccipdFilteringGroups_h



/**
 * \defgroup Filtering Filtering
 * 
 * \brief Image filtering / feature extraction.
 * 
 * Filtering contains code for
 * filtering images.
 * 
 * For example, extracting
 * the standard deviation
 * texture feature from an image
 * goes into the Filtering library.
 */

/**
 *\defgroup FeatureCollection FeatureCollection
 * 
 * \brief Feature storing library.
 * 
 * FeatureCollection is a full library
 * aimed at making using image features
 * easier.
 * 
 * For example, it contains classes
 * to extract a whole series of
 * texture features from a single volume
 * and to cache the results, so that
 * the next time the same feature
 * is requested, it won't be
 * recalculated.
 * 
 * \ingroup Filtering
 */



#endif // __ccipdFilteringGroups_h
