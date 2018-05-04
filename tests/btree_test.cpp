#include "gtest/gtest.h"


extern "C" 
{
#include "bplustree.h"
}
//#include <random>
//#include <map>



TEST( btree, constuction )
{
    struct bplus_tree *tree = bplus_tree_init( "/tmp/leoma-btree.index", 128 );
    ASSERT_TRUE( tree );
    bplus_tree_deinit( tree );
}

#if 0
TEST( btree, search_empty )
{
    const std::size_t order = 4;
    BPlusTree tree( order );
    Record* rec = tree.search( 1 );
    ASSERT_TRUE( !rec );
}


TEST( btree, one_item )
{
    const std::size_t order = 4;
    const int64_t k = 17;

    BPlusTree tree( order );

    ASSERT_NO_THROW( tree.insert( k, k ) );
    Record* rec = tree.search( k );
    ASSERT_TRUE( rec != nullptr );
    ASSERT_TRUE( rec->value() == k );

    tree.remove( k );

    ASSERT_TRUE( tree.search( k ) == nullptr  );
    ASSERT_TRUE( tree.is_empty() );
}

/*
TEST( btree, more_items )
{
    const std::size_t order = 10;
    const std::size_t item_no = 2000;
    BPlusTree tree( order );

    for( std::size_t i = 0; i < item_no; i++ )
    {
        ASSERT_NO_THROW( tree.insert( i, KeyType( i ) ) );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        Record* rec = tree.search( i );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == KeyType( i ) );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.remove( i );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        //std::cout << i << " " << std::flush;
        ASSERT_TRUE( tree.search( i ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );

}
*/


TEST( btree, reverse_erase )
{
    const std::size_t order = 5;
    const std::size_t item_no = 20000;
    BPlusTree tree( order );

    for( std::size_t i = 0; i < item_no; i++ )
    {
        ASSERT_NO_THROW( tree.insert( KeyType( i ), i ) );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        Record* rec = tree.search( i );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == ValueType( i ) );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        tree.remove( item_no - i - 1 );
    }

    for( std::size_t i = 0; i < item_no; i++ )
    {
        // std::cout << i << " " << std::flush;
        ASSERT_TRUE( tree.search( i ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );

}

/*
TEST( btree, insert_search_delete )
{
    const std::size_t item_no = 9000;
    const unsigned seed = 12345;
    std::map< KeyType, ValueType > smap;
    std::srand( seed );
    const std::size_t order = 5;

    BPlusTree tree( order );

    for( std::size_t i = 0; i < item_no; i++ )
    {
        const KeyType key( std::rand() );
        // std::cout << key.get_value() << " " << std::flush;
        smap.insert( std::make_pair( key, i ) );
        ASSERT_NO_THROW( tree.insert( key, i ) );
    }

    for( auto v : smap )
    {
        Record* rec = tree.search( v.first );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == v.second );
    }

    for( auto v : smap )
    {
        tree.remove( v.first );
    }

    for( auto v : smap )
    {
        ASSERT_TRUE( tree.search( v.first ) == nullptr );
    }

    ASSERT_TRUE( tree.is_empty() );
}
*/

TEST( btree, insert_random )
{
    std::map< KeyType, ValueType > smap;
    const std::size_t order = 5;
    BPlusTree tree( order );

    const double threshold = 0.6;
    const std::size_t iter_no = 2000;

    //std::random_device rd; // only used once to initialise (seed) engine
    //std::mt19937 rng( rd() ); // random-number engine used (Mersenne-Twister in this case)
    std::mt19937 rng;
    std::uniform_real_distribution<> dist_real( 0, 1 );

    std::uniform_int_distribution< int > dist_int( 0, iter_no/2 );

    for( std::size_t i = 0; i < iter_no; i++ )
    {
        const KeyType key( dist_int( rng ) );
        const double x = dist_real( rng );

        if( x > threshold )
        {
            const auto ret = smap.insert( std::make_pair( key, i ) );
            if( ret.second )
            {
                ASSERT_NO_THROW( tree.insert( key, i ) );
            }
            else
            {
                ASSERT_ANY_THROW( tree.insert( key, i ) );
            }
        }
        else
        {
            smap.erase( key );
            tree.remove( key );
        }

        // std::cout<< sset.size() << "\n" << std::flush;

        for( auto v : smap )
        {
            Record* rec = tree.search( v.first );
            ASSERT_TRUE( rec );
            ASSERT_TRUE( rec->value() == v.second );
        }

    }
}


TEST( btree, erase_random )
{
    std::map< KeyType, ValueType > smap;
    const std::size_t order = 5;
    BPlusTree tree( order );

    const std::size_t iter_no = 5000;

    //std::random_device rd; // only used once to initialise (seed) engine
    // std::mt19937 rng( rd() ); // random-number engine used (Mersenne-Twister in this case)
    std::mt19937 rng;

    std::uniform_int_distribution< int > dist_int( 0, iter_no/2 );

    for( std::size_t i = 0; i < iter_no; i++ )
    {
        const int64_t key( dist_int( rng ) );
        const auto ret = smap.insert( std::make_pair( key, i ) );
        if( ret.second )
        {
            ASSERT_NO_THROW( tree.insert( key, i ) );
        }
        else
        {
            ASSERT_ANY_THROW( tree.insert( key, i ) );
        }
    }

    for( auto v : smap )
    {
        Record* rec = tree.search( v.first );
        ASSERT_TRUE( rec );
        ASSERT_TRUE( rec->value() == v.second );
    }

    while( !smap.empty() )
    {
        const KeyType key( dist_int( rng ) );
        smap.erase( key );
        tree.remove( key );
    }

    ASSERT_TRUE( tree.is_empty() );
}


#endif

