#ifndef MODULE_EVENTS_H
#define MODULE_EVENTS_H

#include <Trackable.h>
#include "../settings/EnumEvent.h"

#include <map>

using std::multimap;
using std::pair;

namespace Modules {

	namespace Messages {

		class Message;
		class Observer;

		typedef multimap<EventType, Observer*> EventMap;
		typedef pair<EventType, Observer*> EventPair;

		class Manager : public Trackable
		{

		private:

			/* The map of event types and observers*/
			EventMap mMap;

			Manager();

			void dispatch_(Message* evt);

			void subscribe_(Observer* observer, EventType type);

			void unsubscribe_(Observer* observer, EventType type);

			void unsubscribe_(Observer* observer);

		public:

			// public for access to lib::remove
			~Manager();

			static Manager* instance();

			// Synonomous to fireEvent - this is the one that is called by outside forces and calls the private event dispatchEvent
			/* Notifies observers of an event's existance */
			static void dispatch(Message* evt);

			// These are synonomous to addListener, removeListener, and removeListenerFromAllEvents
			/* Subscribes an observer to all events of a specific type */
			static void subscribe(Observer* observer, EventType type);

			/* Subscribes an observer to all events of specific types */
			static void subscribe(Observer* observer, EventType types[], int size);

			/* Unsubscribes an observer to all events of a specific type */
			static void unsubscribe(Observer* observer, EventType type);

			/* Unsubscribes an observer to all events */
			static void unsubscribe(Observer* observer);

		};

	};

};

#endif