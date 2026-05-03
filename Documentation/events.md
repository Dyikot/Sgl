# Events

## What are Events and Delegates?
Delegates are type-safe function wrappers that can store and invoke any callable object—free functions, lambdas, or member methods—with a specific signature. They enable loose coupling by allowing objects to notify others without knowing their concrete types.

Events are multicast delegates: a collection of handlers that are all invoked when the event is raised. This pattern is fundamental for reactive UI programming, where controls notify the application about user interactions, state changes, or lifecycle events.

## Key Concepts
| Term | Description |
| -- | -- |
| Delegate | A single callable wrapper with a fixed signature `(TReturn(TArgs...))` |
| Event | A container holding multiple delegates; invokes all when raised |
| Handler | A function, lambda, or method subscribed to an event |
| Sender | The object raising the event (passed to handlers for context) |
| EventArgs | Data structure carrying event-specific information to handlers |

## Delegate implementation
`Delegate<TReturn(TArgs...)>` is a type-erased callable wrapper built with C++20 features:
- **Type erasure:** Store any callable (lambda, function pointer, functor, bound method) matching the signature
- **Copyable & movable:** Supports standard value semantics
- **Equality comparison:** Handlers can be compared for removal (`operator==`)
- **Type introspection:** `TargetType()` returns `std::type_info` of the stored callable

### Common type aliases
```c++
// Void-returning delegate (e.g., for actions)
using Action = Delegate<void(TArgs...)>;

// Boolean-returning delegate (e.g., for predicates/filters)
using Predicate = Delegate<bool(TArgs...)>;

// Value-returning delegate
using Func<TResult> = Delegate<TResult(TArgs...)>;
```

### Example
```c++
// Create a delegate wrapping a lambda
Delegate<void(int)> callback = [](int value) 
{
    std::cout << "Value: " << value << "\n";
};

// Invoke the delegate
if(callback.HasTarget()) {
    callback(42);  // Outputs: Value: 42
}
```

## Event implementation
The `Event<EventHandler<TSender, TEventArgs>>` class manages a collection of handlers and provides a clean subscription API.

### Event API Overview
| Method/Operator | Purpose |
| -- | -- |
| `operator+=` | Subscribe a handler to the event |
| `operator-=` | Unsubscribe a previously added handler |
| `Invoke(sender, args)` | Raise the event, calling all registered handlers |
| `HasHandlers()` | Check if any handlers are subscribed |
| `Count()` | Return the number of subscribed handlers |
| `Clear()` | Remove all handlers |

### EventHandler
Sgl defines a standard event handler signature:
```c++
template<typename TSender, typename TEventArgs = EventArgs>
using EventHandler = Delegate<void(TSender&, TEventArgs)>;
```
- `TSender`: Type of the object raising the event (e.g., `Button`, `Window`)
- `TEventArgs`: Custom data structure (defaults to empty `EventArgs`)

### EventArgs
Use EventArgs (or a custom struct) to pass event-specific data:
```c++
// Custom event args for a click event with position
struct ClickEventArgs 
{
    float X;
    float Y;
};

// Create an alias for the event handler
using ClickEventHandler = EventHandler<Button, ClickEventArgs>;

// Event declaration
Event<ClickEventHandler> Clicked;
```

### Subscribing to Events
#### Example 1: Lambda Handler
The most common pattern for simple, inline logic:
```c++
button->Click += [](Button& sender, EventArgs e)
{
    sender.SetContent("Clicked");
};
```
#### Example 2: Member Method Handler
For reusable logic or when lambdas become complex, use `MethodEventHandler`:
```c++
button->Click += MethodEventHandler(&MyWindow::OnButtonClick, this);
```
#### Example 3: Unsubscribing Handlers
Store the handler to remove it later:
```c++
auto handler = [](auto& sender, auto e) { /* ... */ };
button->Click += handler;
button->Click -= handler;
```
For member methods, equality comparison works automatically:
```c++
button->Click += MethodEventHandler(&MyWindow::OnButtonClick, this); // Subscribe
button->Click -= MethodEventHandler(&MyWindow::OnButtonClick, this); // Unsubscribe
```

### Raising Events
Events are typically raised by the element that owns them.
```c++
CustomEvent.Invoke(*this, EventArgs());
```
If `TEventArgs` is default-constructible, you can omit the args parameter:
```c++
CustomEvent.Invoke(*this);
```

## Best Practices
Do:
- Use lambdas for simple, short-lived handlers
- Use member methods for complex logic or when handlers need to be unsubscribed
- Capture `this` weakly or unsubscribe in destructors to avoid dangling references

Avoid:
- Subscribing the same handler multiple times (leads to duplicate calls)
- Modifying the event container while iterating (e.g., subscribing inside a handler)
- Assuming thread safety—always marshal to the UI thread via `Dispatcher` for cross-thread scenarios