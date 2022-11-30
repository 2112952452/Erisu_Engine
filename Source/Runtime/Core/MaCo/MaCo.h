//
// Created by Adarion on 2022/12/8.
//

#ifndef ADARION_MACO_H
#define ADARION_MACO_H

#include <any>
#include <list>
#include <future>
#include <variant>
#include <optional>
#include <coroutine>
#include <exception>
#include <functional>
#include <condition_variable>

#include <Log/LogSystem.h>
#define MACO_LOG(...) LOG_ERROR(__VA_ARGS__)

namespace MaCoroutine
{
    template<typename T>
    requires (!std::is_void_v<T> && !std::is_reference_v<T>)
    class Generator
    {
    public:
        struct GeneratorPromise
        {
            T currentValue_;
            bool isReady_ = false;

            std::suspend_always yield_value(T value)
            {
                currentValue_ = value;
                isReady_ = true;
                return {};
            }

            std::suspend_always initial_suspend()
            { return {}; }

            std::suspend_always final_suspend() noexcept
            { return {}; }

            void return_void()
            {}

            Generator get_return_object()
            { return Generator{Handle::from_promise(*this)}; }

            void unhandled_exception()
            { MACO_LOG("[MaCoroutine::Generator] Unhandled exception\n"); }
        };

    private:
        using Handle = std::coroutine_handle<GeneratorPromise>;

        explicit Generator(Handle h) : handle_(h)
        {}

    private:
        Handle handle_;

    public:
        using promise_type = GeneratorPromise;

        Generator(const Generator &) = delete;

        Generator &operator=(const Generator &) = delete;

        Generator(Generator &&other) noexcept: handle_(std::exchange(other.handle_, {}))
        {}

        ~Generator()
        {
            if (handle_)
            { handle_.destroy(); }
        }

        Generator &operator=(Generator &&other) noexcept
        {
            if (this != &other)
            {
                if (handle_) handle_.destroy();
                handle_ = other.handle_;
                other.handle_ = nullptr;
            }
            return *this;
        }

    public:
        std::optional<T> Next() noexcept
        {
            return HasNext() ? handle_.promise().isReady_ = false, std::make_optional(handle_.promise().currentValue_)
                             : std::nullopt;
        }

        bool Next(T &value) noexcept
        {
            if (HasNext())
            {
                value = handle_.promise().currentValue_;
                handle_.promise().isReady_ = false;
                return true;
            }
            return false;
        }

    private:
        [[nodiscard]] bool HasNext() const noexcept
        {
            if (handle_.done())
                return false;

            if (!handle_.promise().isReady_)
                handle_.resume();

            return !handle_.done();
        }

    public:
        static Generator<T> FromArray(T *arr, size_t size)
        {
            for (size_t i = 0; i < size; ++i)
            { co_yield arr[i]; }
        }

        // Container must have begin() and end() method, and the value type must be T
        template<typename Container, typename = std::enable_if_t<
                std::is_same_v<decltype(std::declval<Container>().begin()), decltype(std::declval<Container>().end())> &&
                std::is_same_v<T, typename Container::value_type>>>
        static Generator<T> FromContainer(Container container)
        {
            for (auto &item: container)
            { co_yield item; }
        }

        template<typename... Args>
        static Generator Create(Args ... args)
        { (co_yield args, ...); }

    public:
        /*
         * Utility Part
         * Usage:
         * Generator<int> g = ...;
         * g.Filter([](int i) { return i % 2 == 0; }).ForEach([](int i) { std::cout << i << std::endl; });
         * g.Map([](int i) { return i * i; }).ForEach([](int i) { std::cout << i << std::endl; });
         * g.Take(10).ForEach([](int i) { std::cout << i << std::endl; });
         * g.TakeWhile([](int i) { return i < 10; }).ForEach([](int i) { std::cout << i << std::endl; });
         *
         * Attention: The order of the above methods is important.
         * And Don't assign the result of the above methods to a variable.
         * Because the result is a temporary object.
         * The following code is wrong:
         * auto g1 = range(1, 1000).Filter([](int i) { return i % 2 == 0; });
         * But the following code is right:
         * auto g1 = range(1, 1000);
         * auto g2 = g1.Filter([](int i) { return i % 2 == 0; });
         */

    public:
        template<typename Func>
        Generator<std::invoke_result_t<Func, T>> Map(Func func)
        {
            while (HasNext())
            { co_yield func(Next().value()); }
        }

        Generator<T> Filter(std::function<bool(T)> func)
        {
            while (HasNext())
            {
                auto value = Next().value();
                if (func(value))
                { co_yield value; }
            }
        }

        Generator<T> TakeWhile(std::function<bool(T)> func)
        {
            while (HasNext())
            {
                auto value = Next().value();
                if (func(value))
                { co_yield value; }
                else
                { break; }
            }
        }

        Generator<T> Take(size_t count)
        {
            while (HasNext() && count--)
            { co_yield Next().value(); }
        }

        Generator<T> SkipWhile(std::function<bool(T)> func)
        {
            while (HasNext())
            {
                auto value = Next().value();
                if (func(value))
                { continue; }
                else
                {
                    co_yield value;
                    break;
                }
            }
            while (HasNext())
            { co_yield Next().value(); }
        }

        Generator<T> Skip(size_t count)
        {
            while (HasNext() && count--)
            { Next(); }
            while (HasNext())
            { co_yield Next().value(); }
        }


        void ForEach(std::function<void(T)> func)
        {
            while (HasNext())
            { func(Next().value()); }
        }


        template<typename Ret, typename Func>
        Ret Fold(Ret init, Func func)
        {
            Ret ret = init;
            while (HasNext())
            { ret = func(ret, Next().value()); }
            return ret;
        }

        T Sum()
        {
            T ret = T();
            while (HasNext())
            { ret += Next().value(); }
            return ret;
        }

        std::vector<T> ToVector()
        {
            std::vector<T> ret;
            while (HasNext())
            { ret.push_back(Next().value()); }
            return ret;
        }

    };

    /*
     *  Task Part
     *
     *  Contains:
     *  Task<T> class
     *  Task<void> class
     *  IExecutor interface
     *
     *  Attentions:
     *  1. Task<T> and Task<void> should using a variable to store the result.
     *  So don't use the following code:
     *  foo().Then([](int i) { std::cout << i << std::endl; });
     *  instead, use the following code:
     *  auto task = foo().Then([](int i) { std::cout << i << std::endl; });
     *  2. co_await can only be used in a coroutine function.
     *  So don't use the following code:
     *  auto task = foo().Then([](int i) { std::cout << i << std::endl; });
     *  co_await task; // wrong, but it can be compiled, if you ensure task is not used after co_await, it will work.
     *  instead, use the following code:
     *  co_await foo().Then([](int i) { std::cout << i << std::endl; });
     */

    class IExecutor
    {
    public:
        virtual ~IExecutor() = default;

        virtual void Execute(std::function<void()> func) = 0;
    };

    class AsyncExecutor : public IExecutor
    {
    public:
        void Execute(std::function<void()> func) override
        {
            auto future = std::async(std::launch::async, func);
        }
    };

    class NewThreadExecutor : public IExecutor
    {
    public:
        void Execute(std::function<void()> func) override
        {
            std::thread(func).detach();
        }
    };

    class SyncExecutor : public IExecutor
    {
    public:
        void Execute(std::function<void()> func) override
        {
            func();
        }
    };


    template<typename T, typename Executor = NewThreadExecutor>
    class Task;

    template<class U, typename Executor>
    struct TaskAwaiter
    {
        TaskAwaiter(Task<U, Executor> &&task, IExecutor *executor) : task_(std::move(task)), executor_(executor)
        {}

        TaskAwaiter(const TaskAwaiter &) = delete;

        TaskAwaiter &operator=(const TaskAwaiter &) = delete;

        TaskAwaiter(TaskAwaiter &&other) noexcept: task_(std::exchange(other.task_, {}))
        {}

        [[nodiscard]] constexpr bool await_ready() const noexcept
        { return false; }

        void await_suspend(std::coroutine_handle<> handle)
        {
            task_.handle_.promise().AddFinallyCallback([handle, this]() mutable
                                              {
                                                  executor_->Execute([handle]() { handle.resume(); });
                                              });
        }

        [[nodiscard]] constexpr U await_resume() const
        { return task_.GetResult(); }

    private:
        Task<U, Executor> task_;
        IExecutor *executor_ = nullptr;
    };

    struct DispatchAwaiter
    {
        explicit DispatchAwaiter(IExecutor *executor) noexcept: executor_(executor)
        {}

        [[nodiscard]] bool await_ready() const
        { return false; }

        void await_suspend(std::coroutine_handle<> handle) const
        {
                   executor_->Execute([handle]()
                               { handle.resume(); });
        }

        void await_resume()
        {}

    private:
        IExecutor *executor_;
    };


    template<typename T, typename Executor>
    class Task
    {
    public:
        struct TaskPromise
        {
            DispatchAwaiter initial_suspend() noexcept
            { return DispatchAwaiter(&executor_); }

            std::suspend_always final_suspend() noexcept
            { return {}; }

            void return_value(T value) noexcept
            {
                std::lock_guard<std::mutex> lock(mutex_);
                result_ = std::move(value);
                cv_.notify_all();

                for (auto &&callback: callbacks_)
                    callback(value);
                callbacks_.clear();

                for (auto &&callback: finallyCallbacks_)
                    callback();
                finallyCallbacks_.clear();
            }

            Task get_return_object() noexcept
            { return Task{Handle::from_promise(*this)}; }

            void unhandled_exception()
            {
                std::lock_guard<std::mutex> lock(mutex_);
                exception_ = std::current_exception();
                cv_.notify_all();

                for (auto &&callback: catchCallbacks_)
                {
                    try
                    { std::rethrow_exception(exception_); }
                    catch (std::exception &e)
                    { callback(e); }
                }
                catchCallbacks_.clear();

                for (auto &&callback: finallyCallbacks_)
                    callback();
                finallyCallbacks_.clear();
            }

            template<typename U, typename E>
            TaskAwaiter<U, E> await_transform(Task<U, E> &&task)
            {
                return TaskAwaiter<U, E>(std::move(task), &executor_);
            }

            template<typename U, typename E>
            TaskAwaiter<U, E> await_transform(Task<U, E> &task)
            {
                return TaskAwaiter<U, E>(std::move(task), &executor_);
            }

            T GetResult()
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (!result_.has_value() && !exception_)
                    cv_.wait(lock);

                if (exception_)
                    std::rethrow_exception(exception_);

                return result_.value();
            }

            void AddCallback(std::function<void(T)> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (result_.has_value())
                {
                    lock.unlock();
                    try
                    { callback(result_.value()); }
                    catch (...)
                    {}
                } else
                    callbacks_.push_back(std::move(callback));
            }

            void AddCatchCallback(std::function<void(std::exception &)> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (exception_)
                {
                    lock.unlock();
                    try
                    { std::rethrow_exception(exception_); }
                    catch (std::exception &e)
                    { callback(e); }
                } else
                    catchCallbacks_.push_back(std::move(callback));
            }

            void AddFinallyCallback(std::function<void()> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (result_.has_value() || exception_)
                {
                    lock.unlock();
                    try
                    { callback(); }
                    catch (...)
                    {}
                } else
                    finallyCallbacks_.push_back(std::move(callback));
            }

        private:
            std::exception_ptr exception_;
            std::optional<T> result_;
            Executor executor_;

            std::condition_variable cv_;
            std::mutex mutex_;

            std::list<std::function<void(T)>> callbacks_;
            std::list<std::function<void(std::exception &)>> catchCallbacks_;
            std::list<std::function<void()>> finallyCallbacks_;
        };

    public:
        using promise_type = TaskPromise;

        Task(const Task &) = delete;

        Task &operator=(const Task &) = delete;

        Task(Task &&other) noexcept: handle_(std::exchange(other.handle_, {}))
        {}

        ~Task()
        {
            if (handle_)
            { handle_.destroy(); }
        }

        Task &operator=(Task &&other) noexcept
        {
            if (this != &other)
            {
                if (handle_) handle_.destroy();
                handle_ = other.handle_;
                other.handle_ = nullptr;
            }
            return *this;
        }

        [[nodiscard]] T GetResult() const
        {
            return handle_.promise().GetResult();
        }

        auto Then(std::function<void(T)> callback)
        {
            handle_.promise().AddCallback(std::move(callback));
            return std::move(*this);
        }

        auto Catch(std::function<void(std::exception &)> callback)
        {
            handle_.promise().AddCatchCallback(std::move(callback));
            return std::move(*this);
        }

        auto Finally(std::function<void()> callback)
        {
            handle_.promise().AddFinallyCallback(std::move(callback));
            return std::move(*this);
        }

    private:
        using Handle = std::coroutine_handle<TaskPromise>;

        friend TaskAwaiter<T, Executor>;

        explicit Task(Handle h) : handle_(h)
        {}

    private:
        Handle handle_;
    };

    template<typename Executor>
    class Task<void, Executor>
    {
    public:
        struct TaskPromise
        {
            DispatchAwaiter initial_suspend() noexcept
            { return DispatchAwaiter(&executor_); }

            std::suspend_always final_suspend() noexcept
            { return {}; }

            void return_void() noexcept
            {
                std::lock_guard<std::mutex> lock(mutex_);
                done_ = true;
                cv_.notify_all();

                for (auto &&callback: callbacks_)
                    callback();
                callbacks_.clear();

                for (auto &&callback: finallyCallbacks_)
                    callback();
                finallyCallbacks_.clear();
            }

            Task get_return_object() noexcept
            {
                return Task{Handle::from_promise(*this)}; }

            void unhandled_exception()
            {
                std::lock_guard<std::mutex> lock(mutex_);
                exception_ = std::current_exception();
                cv_.notify_all();

                for (auto &&callback: catchCallbacks_)
                {
                    try
                    { std::rethrow_exception(exception_); }
                    catch (std::exception &e)
                    { callback(e); }
                }
                catchCallbacks_.clear();

                for (auto &&callback: finallyCallbacks_)
                    callback();
                finallyCallbacks_.clear();
            }

            template<typename U, typename E>
            TaskAwaiter<U, E> await_transform(Task<U, E> &&task)
            {
                return TaskAwaiter<U, E>(std::move(task), &executor_);
            }

            template<typename U, typename E>
            TaskAwaiter<U, E> await_transform(Task<U, E> &task) // it will cause original task to be moved
            {
                return TaskAwaiter<U, E>(std::move(task), &executor_);
            }

            void AddCatchCallback(std::function<void(std::exception &)> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (exception_)
                {
                    lock.unlock();
                    try
                    { std::rethrow_exception(exception_); }
                    catch (std::exception &e)
                    { callback(e); }
                } else
                    catchCallbacks_.push_back(std::move(callback));
            }

            void AddFinallyCallback(std::function<void()> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (done_ || exception_)
                {
                    lock.unlock();
                    try
                    { callback(); }
                    catch (...)
                    {}
                } else
                    finallyCallbacks_.push_back(std::move(callback));
            }

            void AddCallback(std::function<void()> callback)
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (done_)
                {
                    lock.unlock();
                    try
                    { callback(); }
                    catch (...)
                    {}
                } else
                    callbacks_.push_back(std::move(callback));
            }

            void GetResult()
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (!done_ && !exception_)
                    cv_.wait(lock);

                if (exception_)
                    std::rethrow_exception(exception_);

            }

        private:
            friend class Task<void, Executor>;
            std::exception_ptr exception_;
            bool done_ = false;
            Executor executor_;

            std::condition_variable cv_;
            std::mutex mutex_;

            std::list<std::function<void()>> callbacks_;
            std::list<std::function<void(std::exception &)>> catchCallbacks_;
            std::list<std::function<void()>> finallyCallbacks_;
        };

    public:
        using promise_type = TaskPromise;

        Task(const Task &) = delete;

        Task &operator=(const Task &) = delete;

        Task(Task &&other) noexcept: handle_(std::exchange(other.handle_, {}))
        {}

        ~Task()
        {
            if (handle_)
            { handle_.destroy(); }
        }

        Task &operator=(Task &&other) noexcept
        {
            if (this != &other)
            {
                if (handle_) handle_.destroy();
                handle_ = other.handle_;
                other.handle_ = nullptr;
            }
            return *this;
        }

        void GetResult() const
        {
            handle_.promise().GetResult();
        }

        auto Then(std::function<void()> callback)
        {
            handle_.promise().AddCallback(std::move(callback));
            return std::move(*this);
        }

        auto Catch(std::function<void(std::exception &)> callback)
        {
            handle_.promise().AddCatchCallback(std::move(callback));
            return std::move(*this);
        }

        auto Finally(std::function<void()> callback)
        {
            handle_.promise().AddFinallyCallback(std::move(callback));
            return std::move(*this);
        }


    private:
        using Handle = std::coroutine_handle<TaskPromise>;

        friend TaskAwaiter<void, Executor>;

        explicit Task(Handle h) : handle_(h)
        {}

    private:
        Handle handle_;
    };

    template<typename T>
    using AsyncTask = Task<T, AsyncExecutor>;

    template<typename T>
    using ThreadTask = Task<T, NewThreadExecutor>;

}// namespace MaCoroutine

#endif//ADARION_MACO_H
