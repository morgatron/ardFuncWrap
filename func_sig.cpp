#include <typeinfo>
#include <boost/bind.hpp>
#include <boost/function.hpp>

template< typename T >
struct identity
{
    typedef T type;
};

// ----------
// Function signature metafunction implementation
// Also handler for function object case
// ----------

template< typename T >
struct function_signature_impl
    : function_signature_impl< decltype( &T::operator() ) >
{
};

// ----------
// Function signature specializations
// ----------

template< typename R >
struct function_signature_impl< R () >
    : identity< R () >
{
};

template< typename R, typename A1 >
struct function_signature_impl< R ( A1 ) >
    : identity< R ( A1 ) >
{
};

template< typename R, typename A1, typename A2 >
struct function_signature_impl< R ( A1, A2 ) >
    : identity< R ( A1, A2 ) >
{
};

// ----------
// Function pointer specializations
// ----------

template< typename R >
struct function_signature_impl< R ( * )() >
    : function_signature_impl< R () >
{
};

template< typename R, typename A1 >
struct function_signature_impl< R ( * )( A1 ) >
    : function_signature_impl< R ( A1 ) >
{
};

// ----------
// Member function pointer specializations
// ----------

template< typename C, typename R >
struct function_signature_impl< R ( C::* )() >
    : function_signature_impl< R () >
{
};

template< typename C, typename R, typename A1 >
struct function_signature_impl< R ( C::* )( A1 ) >
    : function_signature_impl< R ( A1 ) >
{
};

template< typename C, typename R >
struct function_signature_impl< R ( C::* )() const >
    : function_signature_impl< R () >
{
};

template< typename C, typename R, typename A1 >
struct function_signature_impl< R ( C::* )( A1 ) const >
    : function_signature_impl< R ( A1 ) >
{
};


// ----------
// Function signature metafunction
// ----------

template< typename T >
struct function_signature
    : function_signature_impl< T >
{
};


// ----------
// Tests
// ----------

template< typename F >
void test( F f )
{
    typedef typename function_signature< F >::type signature_type;

    std::cout << typeid( F ).name() << std::endl;
    std::cout << '\t' << typeid( signature_type ).name() << std::endl;
    std::cout << std::endl;
}


int foo( float)
{
    return 0;
}

struct bar
{
    int operator ()( int )
    {
        return 0;
    }

};

struct cbar
{
    int operator ()( int ) const
    {
        return 0;
    }

};

struct abar1
{
    int operator ()( int ) const
    {
        return 0;
    }

    int operator ()( int )
    {
        return 0;
    }

};

struct abar2
{
    int operator ()( int )
    {
        return 0;
    }

    int operator ()( double )
    {
        return 0;
    }

};

struct mem
{
    int f( int ) const
    {
        return 0;
    }
};


int main()
{
    test(
        []( int ) -> int { return 0; }
    );

	std::cout << "foo" << std::endl;
    test(
        foo
    );

    test(
        &foo
    );

	std::cout << "bar" << std::endl;
    test(
        bar()
    );

	std::cout << "cbar" << std::endl;
    test(
        cbar()
    );

    test(
        std::function< int ( int ) >( &foo )
    );

    test(
        boost::function< void ( int ) >( &foo )
    );

    /*
    test(
        std::bind( &mem::f, mem(), std::placeholders::_1 )
    );
    */

    /*
    test(
        boost::bind( &mem::f, mem(), _1 )
    );
    */

    /*
    test(
        abar1()
    );
    */

    /*
    test(
        abar2()
    );
    */

    return EXIT_SUCCESS;
}
