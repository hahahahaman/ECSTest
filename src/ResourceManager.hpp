/*
ResourceManager.hpp
2013-03-01-01.06.06
*/

/**
MIT License:

Copyright (c) 2008 Adam W. Brown (www.darkrockstudios.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>

namespace ye
{
template< class T >
class ResourceManager
{
    public:
        typedef std::pair< std::string, T* >    Resource;
        typedef std::unordered_map< std::string, T* >   ResourceMap;

    protected:
        ResourceMap m_resource;

        T* find( const std::string& strId )
        {
            T* resource = NULL;
            typename ResourceMap::iterator it = m_resource.find( strId );
            if( it != m_resource.end() )
            {
                resource = it->second;
            }

            return resource;
        }

        virtual T* load( const std::string& strId ) = 0;

    public:
        ResourceManager()
        {

        }

        virtual ~ResourceManager()
        {
            releaseAllResources();
        }

         /**if ID not found, then the resource is loaded*/
        virtual T* getResource( const std::string& strId )
        {
            T* resource = find( strId );
            if( resource == NULL )
            {
                resource = load( strId );
                // If the resource loaded successfully, add it to the resource map
                if( resource != NULL )
                    m_resource.insert( Resource( strId, resource ) );
            }
            return resource;
        }

        void releaseResource( const std::string& strId)
        {
            T* resource = find( strId );
            if( resource != NULL )
            {
                delete resource;
                m_resource.erase( m_resource.find( strId ) );
            }
        }

        void releaseAllResources()
        {
            while( m_resource.begin() != m_resource.end() )
            {
                delete m_resource.begin()->second;
                m_resource.erase( m_resource.begin() );
            }
        }
};
}

#endif // RESOURCEMANAGER_H_INCLUDED
