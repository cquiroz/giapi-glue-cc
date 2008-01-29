#ifndef STATUSDATABASE_H_
#define STATUSDATABASE_H_
#include <ext/hash_map>

#include <log4cxx/logger.h>

#include <giapi/giapi.h>
#include <giapi/giapiexcept.h>
#include "StatusItem.h"

//hash_map is an extension of STL widely available on gnu compilers, fortunately 
//Will make its namespace visible here. 
using namespace __gnu_cxx;

namespace giapi {
/**
 * The status database is a repository of all the status items in the system.
 * Provides mechanisms to handle different operation over the stored items
 */
class StatusDatabase {
	/**
	 * A comparator for strings to be used in the definition of the 
	 * hash_table the database uses internally
	 */
	struct eqstr {
		bool operator()(const char *s1, const char *s2) const {
			return strcmp(s1, s2) == 0;
		}
	};
	/**
	 * Type definition for the hash_table that will map strings to 
	 * StatusItems
	 */
	typedef hash_map<const char *, StatusItem *, hash<const char *>, eqstr>
			StringStatusMap;

	/**
	 * Logging facility
	 */
	static log4cxx::LoggerPtr logger;

private:
	StringStatusMap _map;
	static StatusDatabase * INSTANCE;
	/**
	 * Private constructor
	 */
	StatusDatabase();
public:
	/**
	 * Get the unique instance of the database
	 * 
	 * @return The StatusDatabase singleton object
	 */
	static StatusDatabase & Instance();

	/**
	 * Create an status item in the database
	 *  
	 * @param name The name of the status item that will be 
	 * 	           created. If an status item with the same name already exists, 
	 *             the method will return giapi::status::ERROR
	 * @param type Type information for the values to be stored in this 
	 *             item. 
	 * 
	 * @return giapi::status::OK if the item was sucessfully created, 
	 *         giapi::status::ERROR if there is an error. 
	 */
	int createStatusItem(const char *name, const type::Type type);

	/**
	 * Create an alarm status item in the status database
	 * 
	 * @param name The name of the alarm status item that will be 
	 * 	           created. If an status item with the same name already exists, 
	 *             the method will return giapi::status::ERROR
	 * @param type Type information for the values to be stored in this 
	 *             item. 
	 * 
	 * @return giapi::status::OK if the item was sucessfully created, 
	 *         giapi::status::ERROR if there is an error. 
	 */
	int createAlarmStatusItem(const char *name, const type::Type type);

	/**
	 * Create a health status item in the status database. A health status 
	 * item provides the overall operational status of a system or subsystem. 
	 * The health can be: health::GOOD, health::WARNING or health::BAD. 
	 * </p>
	 * The default state for health after creation is health::GOOD.   
	 *  
	 * @param name The name of the health status item that will be 
	 * 	           created. If an status item with the same name already exists, 
	 *             the method will return giapi::status::ERROR
	 * 
	 * @return giapi::status::OK if the item was sucessfully created, 
	 *         giapi::status::ERROR if there is an error. 
	 */
	int createHealthStatusItem(const char *name);

	/**
	 * Set the health value for the given health status item
	 * 
	 * @param name Name of the health item. The health item must have been 
	 *             initialized by a call to {@link #createHealthStatusItem()}. 
	 *             Failing to do so will return an error. 
	 *             
	 * @param health the health state of the health status item specified by 
	 *               name
	 * 
	 * @return giapi::status::OK if the health was sucessfully set 
	 *         giapi::status::ERROR if there was an error setting the health 
	 *         (for instance, the health status item hasn't been created or
	 *         the name doesn't correspond to a health status item).  
	 * 
	 * @see giapi::health::Health
	 * 
	 */
	int setHealth(const char *name, const health::Health health);

	/**
	 * Set the value of the given status item to the provided 
	 * integer value.
	 * 
	 * @param name Name of the status item whose value will be set
	 * @param value Integer value to store in the status item. 
	 * 
	 * @return giapi::status::OK if the value was set correctly 
	 *         giapi::status::WARNING if the current status value is
	 *         already set to the new value. The StatusItem will not 
	 *         be marked as dirty in this case. 
	 *         giapi::status::ERROR  if there is a problem setting the 
	 *         value and the operation was aborted. This can happen if the 
	 *         type of the status item was not defined as type::INTEGER, 
	 *         if there is no StatusItem associated to the <code>name</code> or
	 *         if <code>name</code> is set to NULL  
	 */
	int setStatusValueAsInt(const char *name, int value);

	/**
	 * Set the value of the given status item to the provided 
	 * string value.
	 * 
	 * @param name Name of the status item whose value will be set
	 * @param value String value to store in the status item. 
	 * 
	 * @return giapi::status::OK if the value was set correctly. The 
	 *         StatusItem is marked dirty. 
	 *         giapi::status::WARNING if the current status value is
	 *         already set to the new value. The StatusItem will not 
	 *         be marked as dirty in this case. 
	 *         giapi::status::ERROR  if there is a problem setting the 
	 *         value and the operation was aborted. This can happen if the 
	 *         type of the status item was not defined as type::STRING, 
	 *         if there is no StatusItem associated to the <code>name</code> or
	 *         if <code>name</code> is set to NULL  
	 * 			  
	 */
	int setStatusValueAsString(const char *name, const char *value);

	/**
	 * Set the alarm for the specified status alarm item. 
	 * 
	 * @param name Name of the alarm item. The alarm items should have been 
	 *             initialized by a call to {@link #createAlarmStatusItem()}
	 *             
	 * @param severity the alarm severity.
	 * @param cause the cause of the alarm 
	 * @param message Optional message to describe the alarm
	 * 
	 * @return giapi::status::OK if alarm was sucessfully set 
	 *         giapi::status::ERROR if there was an error setting the alarm. This 
	 *         happens for instance if the alarm status item has not been 
	 *         created or the name is associated to an status item without 
	 *         alarms).
	 *        
	 * @see alarm::Severity
	 * @see alarm::Cause
	 */
	int setAlarm(const char *name, const alarm::Severity severity,
			const alarm::Cause cause, const char *message = 0);

	/**
	 * Clear the alarm state of the alarm status item specified 
	 * by name. 
	 * 
	 * @param name Name of the alarm item. The alarm items should have been 
	 *             initialized by a call to {@link #createAlarmStatusItem()}. 
	 *             Failing to do so will return an error
	 * @return giapi::status::OK if the alarm was cleared
	 *         giapi::status::ERROR if there was an error clearing the alarm
	 *         (for instance, the alarm has not been created, or the name is
	 *         not associated to an alarm status item)
	 */
	int clearAlarm(const char *name);

	/**
	 * Return the status item associated to the given key, if found.
	 * 
	 * @param name Name of the status item to retrieve
	 * 
	 * @return the StatusItem object associated to the name, or 
	 * NULL if not found
	 */
	StatusItem* getStatusItem(const char *name);

	virtual ~StatusDatabase();

};
}
#endif /*STATUSDATABASE_H_*/
