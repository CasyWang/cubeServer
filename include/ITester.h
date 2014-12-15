#ifndef ITESTER_H
#define ITESTER_H

namespace cubeServer
{
    namespace ITester
    {
        class ITester
        {
        public:
            ITester() {}
            virtual ~ITester() {}
            virtual void doBurnTestFw() {}
            virtual void doBurnFactoryFw() {}
            virtual void doBurnIcBootloader() {}
            virtual void doBurnBoardBootloader() {}
        protected:
        private:
        };
    }
}



#endif // ITESTER_H
