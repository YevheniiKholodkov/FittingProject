
#ifndef OArrayH
#define OArrayH

#ifdef _MSC_VER
#pragma warning(disable: 4068) // душим VC:      C4068 unknown pragma
#pragma warning(disable: 4995) // душим VC:      C4995: 'sprintf': name was marked as #pragma deprecated	
#elif  __BORLANDC__
#pragma warn-8027              // душим builder: non inline functions
#endif


//========================================================================================
//                                        OArray
//========================================================================================
//
template <class T>  struct OArray
{
   OArray()                   { _begin=0; _size=_capacity=0;               } //!< Конструктор по-умолчанию
   OArray(const OArray<T> &a) { _begin=0; _size=_capacity=0; operator=(a); } //!< Конструктор создающий копию массива a
   OArray(int num)            { _begin=0; _size=_capacity=0; create(num);  } //!< Конструктор создающий num элементов Size()==num
   OArray(int num, const T &v){ _begin=0; _size=_capacity=0; create(num,v);} //!< Конструктор создающий num элементов Size()==num и присваивающий им значение v

   virtual ~OArray(){ freeAll(); }                                                    //!< освобождение объекта и его удаление

   void create(int num);                                                     //!< cоздать массив с num элементами
   void create(int num, const T &val){ create(num); set(val);}               //!< cоздать массив с num элементами и проинициализировать их значением val
   void reserve(int new_capacity);                                           //!< изменить зарезервированный размер памяти (вместимость массива)
   void clear(){_size=0;}                                                    //!< размер нулевой, но память (capacity) не освобождает
   void freeAll(){ if(_begin) delete[]_begin; _begin=0;_size=_capacity=0;}    //!< полное освобождение памяти

   inline void push(const T& val);                                           //!< добавить элемент val в конец (если надо, увеличить память, удвоив ее).
   inline void push();                                                       //!< добавляет "пустой" элемент без его инициализации
   inline T& push_back(const T& val);                                        //!< то же, что и push(const T& val); только возвращает ссылку на добавленный элемент
   inline T& push_back();                                                    //!< то же, что и push(); только возвращает ссылку на добавленный элемент
   inline int  add (T& val, int add=16);                                    //!< добавить элемент val в конец (если надо, увеличив память на add элементов). Возвращает индекс последнего эемента массва = _size-1
   inline T &  pop ();                                                       //!< вытолкнуть последний элемент, уменьшив _size

   int         size() const {return _size;}                                //!< число элементов в массивеv == Count()
   inline void size    (int new_size);                                       //!  переустановить размер (если capacity<new_size то увеличить capacity=new_size)
   void        addSize (int add_size) { size(size()+add_size);}              //!< добавить к текущему размеру массива величину add_size (удобен для добавления строк в "двумерных" массивах)

   int         count   () const  { return _size;     }                             //!< число элементов в массиве == Size()
   void        count   (int new_size) { size(new_size); }                    //!  переустановить размер (если capacity<new_size то увеличить capacity=new_size)
   void        addCount(int add_size) { size(size()+add_size);}              //!< доавить к текущему размеру массива величину add_size (удобен для добавления строк в "двумерных" массивах)

   int         capacity()  { return _capacity; }                             //!< емкость массива (зарезервированное по памяти число элементов)
   bool        empty   () const { return _size==0;  }                             //!< массив не содержит элементов

   void        deleteItems(int item, int num=1);                            //!< удалить num элементов начиная с номера item (все сдвинуть влево)
   void        insert(const T& val, int item=0);                            //!< вставить на место item, раздвинув все вправо (долго). По умолчанию вставляет в начало. При необходимости память удваивает.
   void        put(const T& val, int item);                                 //!< вставить на место item (заменить) или добавить в конец
   void        erase (int item, int num);                                   //!< то-же, что и deleteItems но происходит не сдвиг а перенос последних num  элементов на место удоляемых - работает быстрее чем Delete но нарушает порядок
   inline void erase (int item);                                            //!< то-же, что и deleteItems но происходит не сдвиг а перенос последнего элемента на место удоляемого - работает быстрее чем Delete но нарушает порядок

   void copy(const T *ptr, int num);                                        //!< скопировать num элементов из памяти

   T *         begin() { return _begin;         }                           //!< указатель на начало (ОСТОРОЖНО! не вызывать delete)
   T *         ptr()   { return _begin;         }                           //!< указатель на начало (ОСТОРОЖНО! не вызывать delete)
   T *         ptr() const { return _begin;     }                           //!< указатель на начало (ОСТОРОЖНО! не вызывать delete)
   T *         end  () { return _begin+_size;    }                          //!< указатель на следующий после последнего элемента.
   T &         front() { return _begin[0]; }                                //!< последний элемент
   T &         back () { return _begin[_size-1]; }                          //!< последний элемент
   const T &   back () const { return _begin[_size - 1]; }                          //!< последний элемент

   T &         at(int i) { return i<=0? _begin[0]: ( i>=_size? _begin[_size-1]: _begin[i]); } // защищенный доступ к массиву
   T &         operator [](int i);											//!< индексный доступ к элементам массива. Не проверяет границы!
   const T&    operator [](int i) const;									//!< индексный доступ к элементам массива. Не проверяет границы!
   T &         operator * ()     { return *_begin;  }                       //!< обращение к первому элементу по ссылке.
   OArray<T>&  operator = (const OArray<T> &arr);                           //!< копирование значений массива
   OArray<T>&  operator = (const T &val) { set(val); return *this; }         //!< присвоить все элементы массива в значение val

   friend bool operator == (const OArray<T> &a1, const OArray<T> &a2){return  equal(a1,a2);}   //!< массивы равны
   friend bool operator != (const OArray<T> &a1, const OArray<T> &a2){return !equal(a1,a2);}   //!< массивы не равны
   friend bool operator <  (const OArray<T> &a1, const OArray<T> &a2){return compare(a1,a2)<0;}//!< массив меньше чем arr
   friend bool operator >  (const OArray<T> &a1, const OArray<T> &a2){return compare(a1,a2)>0;}//!< массивы больше чем arr

   void   operator << (const OArray<T> &arr);                        //!< присоедиение массива arr в конец текущего
   void   operator += (const OArray<T> &arr);                        //!< поэлементное прибавление
   void   operator -= (const OArray<T> &arr);                        //!< поэлементное вычитание

   // функции алгоритмов работают в интервале [lf,rt]. Каждая функция имеет версию для всего диапозона
   //
   void set       (const T& val);                              //!< установить все элементы в значение val
   void set       (const T& val, int lf, int rt);              //!< установить все элементы между [lf..rt] в значение val

   int  find      (const T& val, int lf, int rt);              //!< ищет первое вхождение в НЕ отсортированном массиве  между [lf..rt]. return -1 если нет элемента val, иначе его индекс
   int  find      (const T& val)                             { return _size? find(val, 0, _size-1): -1; } //!< ищет первое вхождение в НЕ отсортированном массиве. \return -1 если нет элемента val, иначе его индекс
   int  numOf     (const T& val);                               //!< ищет количество вхождений элемент в массив

   int  findBinary(const T& val, int lf, int rt);              //!< ищет первое вхождение в ОТСОРТИРОВАННОМ массиве между [lf..rt]. return -1 если нет элемента val, иначе его индекс
   int  findBinary(const T& val)                             { return _size? findBinary(val, 0, _size-1) : -1; }  //!< ищет первое вхождение в ОТСОРТИРОВАННОМ массиве. \return -1 если нет элемента val, иначе его индекс

   void invert    (int lf, int rt);                            //!< проинвертировать элементы массива между [lf..rt]: {1,2,3}->{3,2,1}
   void invert    ()                                         { if(_size) invert(0, _size-1);}    //!< проинвертировать элементы массива: {1,2,3}->{3,2,1}

   void sort      (int lf, int rt);                            //!< отсортировать элементы массива между [lf..rt]
   void sort      ()                                         { if(_size) sort(0, _size-1); }     //!< отсортировать элементы массива
   void sort      (int lf, int rt, bool (*Less)(const T &a, const T &b));                        //!< отсортировать элементы массива  между [lf..rt] применяя функцию сортировки Less
   void sort      (bool (*Less)(const T &a, const T &b))     { if(_size) sort(0, _size-1, Less);}//!< отсортировать все элементы массива применяя функцию сортировки Less

   void mergeSort (bool (*Comparator)(const T& a, const T& b));
   void mergeSort (int left, int right, bool (*Comparator)(const T& a, const T& b));
   void mergeSort (int left, int right, bool (*Comparator)(const T& a, const T& b), T* temp);

   void group     (bool (*Comparator)(const T& a, const T& b));
   void group     (int lf, int rt, bool (*Comparator)(const T& a, const T& b));

   void groupToBegin(bool (*Predicate)(const T& a, const T& b));
   void groupToBegin(int lf, int rt, bool (*Predicate)(const T& a, const T& b));

   void shuffle();

protected:
   void merge     (int left, int split, int right, bool (*Comparator)(const T& a, const T& b), T* temp);
public:
   void sortBubble(bool (*Less)(const T &a, const T &b));	//!< пузырьковая сортировка с компаратором

   void transform (int lf, int rt, void (*Act)(T &a));         //!< произвести преобразование с каждым элементы массива между [lf..rt] применяя к ним функцию Act
   void transform (void (*Act)(T &a))                        { if(_size) transform(0, _size-1, Act);} //!< произвести преобразование с каждым элементы массива применяя к ним функцию Act

   T    sum       (int lf, int rt);                            //!< Просуммировать элементы массива между [lf..rt]
   T    sum       ()                                         { return sum(0, _size-1); };  //!< Просуммировать все элементы массива

   T    sumMod    (int lf, int rt, const T module);            //!< Просуммировать элементы массива по модулю module между [lf..rt]
   T    sumMod    (const T module)                           { return sumMod(0, _size-1, module); }  //!< Просуммировать все элементы массива по модулю module

   friend bool equal  (const OArray<T> &a1, const OArray<T> &a2)    //! массивы равны
   {
      if(a1._size!=a2._size) return 0;
      T *c1=a1._begin, *e=a1._begin+a1._size, *c2=a2._begin;
      while(c1<e)
         if(*c1++ != *c2++)
            return 0;
       return 1;
   }
   friend int  compare(const OArray<T> &a1, const OArray<T> &a2)    //! (-1,0,1)=(меньше,равны,больше)
   {
      T *c1=a1._begin, *end1=a1._begin+a1._size,
        *c2=a2._begin, *end2=a2._begin+a2._size;
      while(c1<end1 && c2<end2)
         if(*c1++ != *c2++)
            break;
      if(c1==end1){
         if(c2==end2) return  0;
         else         return -1;
      }
      else if(c2==end2) return 1;
      else return (*c1 < *c2)? -1: 1;
   }

protected:
   T  *_begin;                                               //!< указатель на начало массива
   int _size;                                                //!< размер массива  (число элементов массива)
   int _capacity;                                            //!< емкость массива (размер выделенной памяти)
};
//========================================================================================
//                              inline Реализация OArray
//========================================================================================
// Создание массива с num элементами
//
template<class T> inline void OArray<T>::create(int num)
{
   if(num==_capacity && num==_size) return;                      // уже создан
   if(_begin) freeAll();                                           // все освобождаем, если было
   if( (_begin = new T[num]) != 0 ) _capacity=_size=num;
}
//========================================================================================
// Оператор присваивания между объектами копирует значения одного массива в другой.
//
template<class T> inline OArray<T>& OArray<T>::operator = (const OArray<T> &arr)
{
   if(arr._begin!=_begin && arr._begin) copy(arr._begin, arr._size);
   else if(arr._begin==0)             freeAll();
   return *this;
}
//========================================================================================
//! добавить элемент val в конец (если надо, увеличить память, удвоив ее)
//
template<class T> inline void OArray<T>::push(const T& val)
{
   if(_size == _capacity)  reserve(2*_capacity);                   // удваиваем память !
   _begin[_size++] = val;
}
//========================================================================================
//! добавляет "пустой" элемент без его инициализации
//
template<class T> inline void OArray<T>::push()
{
   if(_size == _capacity)  reserve(2*_capacity);                   // удваиваем память !
   _size++;
}
//========================================================================================
//! то же, что и push(const T& val); только возвращает ссылку на добавленный элемент
//
template<class T> inline T& OArray<T>::push_back(const T& val)
{
   push(val);
   return back();
}
//========================================================================================
//! то же, что и push(); только возвращает ссылку на добавленный элемент
//
template<class T> inline T& OArray<T>::push_back()
{
   push();
   return back();
}
//========================================================================================
//! добавить элемент val в конец (если надо, увеличив память на add элементов)
//! \return индекс последнего эемента массва = _size-1
//
template<class T> inline int OArray<T>::add(T& val, int add)
{
   if(_size == _capacity)  reserve(_capacity+add);                 // удваиваем память !
   _begin[_size++] = val;
   return _size-1;
};
//========================================================================================
// вытолкнуть последний элемент, уменьшив _size
//
template<class T> inline T & OArray<T>::pop ()
{
   if(_size==0) return *_begin;                                  // \todo что делать?
   return _begin[--_size];
}
//========================================================================================
//! переустановить размер (если capacity<new_size то увеличить capacity=new_size)
//
template<class T> inline void OArray<T>::size(int new_size)
{
   if(new_size>_capacity) reserve(new_size);
   _size=new_size;
}
//========================================================================================
//                               non inline Реализация OArray
//========================================================================================
// скопировать num элементов из памяти
//
template<class T> void OArray<T>::copy(const T *ptr, int num)
{
   if(_capacity<num){
      freeAll();                                                  // освобождаем старые данные
      if((_begin = new T[num])==0 ){ _capacity=_size=0; return; } // выделяем новую память \todo
      _capacity = num;
   }
   const T *p=ptr, *_end = p+num;
   T *k=_begin;
   while(p<_end) *k++ = *p++;                                   // копируем значения!
   _size  = num;
}
//========================================================================================
// изменить зарезервированный размер памяти (вместимость массива)
//
template<class T> void OArray<T>::reserve(int new_capacity)
{
   if(new_capacity <= 0) new_capacity=16;                     // см. Push,Insert-пустой имеет capacity==0
   T* tmp = _begin;
   _begin = new T[new_capacity];
   if (_begin != 0 )
   {                   // копируем данные в новую память
      int num = (new_capacity < _size)? new_capacity: _size;
      if(tmp){
         T* k=tmp, *kEnd=k+num, *p=_begin;
         while( k<kEnd ) *p++ = *k++;
         delete [] tmp;
      }
      _size     = num;
      _capacity = new_capacity;
   }
   else 
	   _begin=tmp;                                             // \todo что делать ?
};
//========================================================================================
// вставить на место item, элемент val, раздвинув все вправо
//
template<class T> void OArray<T>::insert(const T& val, int item)
{
   if(item>=0 && item<_size){
      if(_size==_capacity) reserve(2*_capacity);
      T* k=_begin+_size-1, *kEnd=_begin+item, *p=_begin+_size;
      for(; k>kEnd; p--, k-- ) *p = *k;
      *p = *k;                                                 // что-бы CodeGuard не цеплялся
      _begin[item] = val;
      _size++;
   } else /*if (_size <= 0)*/ {
      abs_assert((item>=0 && item<=_size) && "OArray<T>::insert (item>=0 && item<=_size)");
      push(val);
   }
};
//========================================================================================
// вставить на место item (заменить) или добавить в конец
//
template<class T> void OArray<T>::put(const T& val, int item)
{
   if (item>=0 && item<_size) {
      _begin[item] = val;
   } else if (item==_size) {
      push(val);
   } else {
      abs_assert((item>=0 && item<=_size) && "OArray<T>::insert (item>=0 && item<=_size)");
   }
};
//========================================================================================
// удалить num элементов начиная с номера item (все сдвинуть влево)
//
template<class T> void OArray<T>::deleteItems (int item, int num)
{
   if(item<0 || item>=_size)
      return;
   if(item+num>=_size) {
      _size=item;
      return;
   }

   T* k=_begin+item+num, *kEnd=_begin+_size, *p=_begin+item;
   while( k<kEnd )
      *p++ = *k++;
   _size-=num;
}
//========================================================================================
// то-же, что и Delete но происходит не сдвиг а перенос последних num  элементов на место удоляемых
// работает быстрее чем Delete но нарушает порядок
//
template<class T> inline void OArray<T>::erase (int item)
{
   if(item<0 || item>=_size)
      return;
   if(item < --_size)
     _begin[item]=_begin[_size];
}
//========================================================================================
// то-же, что и Delete но происходит не сдвиг а перенос последних num  элементов на место удоляемых
// работает быстрее чем Delete но нарушает порядок
//
template<class T> void OArray<T>::erase (int item, int num)
{
   if(item<0 || item>=_size)
      return;
   if(item+num>=_size){
      _size=item;
      return;
   }

   T *p=_begin+item, *kEnd=_begin+_size;
   T *k = (item+num<_size-num)?  kEnd-num: kEnd-item+num;

   while( k<kEnd )
      *p++ = *k++;
   _size-=num;
}
//========================================================================================
//!< поэлементное прибавление
//
template<class T> void OArray<T>::operator += (const OArray<T> &arr)
{
   int num = _size<arr._size? _size: arr._size;
   T *v=_begin, *end=v+num, *v1=arr._begin;
   for(; v<end; v++, v1++) (*v) += (*v1);
}
//========================================================================================
//!< поэлементное вычитание
//
template<class T> void OArray<T>::operator -= (const OArray<T> &arr)
{
   int num = _size<arr._size? _size: arr._size;
   T *v=_begin, *end=v+num, *v1=arr._begin;
   for(; v<end; v++, v1++) (*v) -= (*v1);
}
//========================================================================================
// присоедиение масcива
//
template<class T> void  OArray<T>::operator << (const OArray<T> &arr)
{
   if(_capacity<_size+arr._size) reserve(_size+arr._size);
   T *it=_begin+_size;
   const T *c=arr._begin, *end=arr._begin+arr._size;
   while(c<end) *it++ = *c++;                                   // копируем в него
   _size += arr._size;                                            // руками увеличиваем длину
}
//========================================================================================
//                                     Алгоритмы:
//========================================================================================
// проинвертировать элементы массива
//
template<class T> void OArray<T>::invert(int lf, int rt)
{
   if(rt>=_size) rt=_size-1;                                     // на всякий случай защита
   for(T *a=_begin+lf, *b=_begin+rt; a<b; a++, b--){
      T tmp=*a; *a=*b; *b=tmp;
   }
}
//==========================================================================================
// Быстрая сортировка рекурсией (Р.Седжвик "Фундаментальные алгоритмы на С++" прог 7.1, 7.2)
// Быстрая сортировка берет некоторый элемент диапозона v=arr[rt], ставит его на "свое" место в
// отсортированной пследовательности и так переставляет оставшиеся элементы, чтобы слева от него
// все были не больше а справа не меньше. Для этого слева (i) и справа (j) мы приближаемся к v
// переставляя местами всех arr[i], arr[j] кто не удовлетворяет этому условию. Затем рекурсионно
// повторяем алгоритм для левого и правого подинтервалов.
// (!) перейти к int индексам без потери производительности нельзя из-за артефактов lf-1 при lf=0.
//
template <class T> void OArray<T>::sort(int lf, int rt)
{
   if(rt<=lf) return;
   T v = _begin[rt];                                            // выбираем элемент разбиения последовательности
   int i=lf-1, j=rt;                                           // индексы в левом и правом от него подинтервалах
   for(;;){
      while(_begin[++i]<v) if(i==rt) break;                     // ищем первый не меньший слева (if - а вдруг v<v==true?)
      while(v<_begin[--j]) if(j==lf) break;                     // ищем первый не меньший справа
      if(i>=j) break;                                          // левая и правая граница сошлись
      T a=_begin[i]; _begin[i]=_begin[j]; _begin[j]=a;             // переставляем их местами
   }
   T a=_begin[i]; _begin[i]=_begin[rt]; _begin[rt]=a;              // cтавим элемент разбиения на место

   if(lf<j) sort(lf, j);                                     // повторяем все для левого поддиапазона
   if(i<rt) sort(i, rt);                                     // повторяем все для правого поддиапазона
}
//==========================================================================================
// Быстрая сортировка рекурсией (Р.Седжвик "Фундаментальные алгоритмы на С++" прог 7.1, 7.2)
//
template <class T> void OArray<T>::sort(int lf, int rt, bool (*Less) (const T &a, const T &b))
{
	// тест - пузырьковая сортировка
	for (int i = 0; i<_size-1; ++i){
		T tmp;
		for (int j=(i+1); j<_size; ++j)
			if (!Less(_begin[i], _begin[j])) {
				tmp = _begin[i];
				_begin[i] = _begin[j];
				_begin[j] = tmp;        
			}
	}
	return;

   if(rt<=lf) return;
   T v = _begin[rt];                                            // выбираем элемент разбиения последовательности
   int i=lf-1, j=rt;                                           // индексы в левом и правом от него подинтервалах
   for(;;){
      while(Less(_begin[++i],v)) if(i==rt) break;               // ищем первый не меньший слева (if - а вдруг v<v==true?)
      while(Less(v,_begin[--j])) if(j==lf) break;               // ищем первый не меньший справа
      if(i>=j) break;                                          // левая и правая граница сошлись
      T a=_begin[i]; _begin[i]=_begin[j]; _begin[j]=a;             // переставляем их местами
   }
   T a=_begin[i]; _begin[i]=_begin[rt]; _begin[rt]=a;              // cтавим элемент разбиения на место

   if(lf<j) sort(lf, j, Less);                               // повторяем все для левого поддиапазона
   if(i<rt) sort(i, rt, Less);                               // повторяем все для правого поддиапазона
   
}
//==========================================================================================
// Merge sort
//
template <class T> 
void OArray<T>::mergeSort(bool (*Comparator)(const T& a, const T& b))
{
	mergeSort(0, _size - 1, Comparator);
}
//==========================================================================================
template <class T> 
void OArray<T>::mergeSort(int left, int right, bool (*Comparator)(const T& a, const T& b))
{
   T* temp = new T[right - left + 1];
   mergeSort(left, right, Comparator, temp);
   delete[] temp;
}
//==========================================================================================
template <class T> 
void OArray<T>::mergeSort(int left, int right, bool (*Comparator)(const T& a, const T& b), T* temp)
{
	int split;                   // индекс, по которому делим массив

	if (left < right)	// если есть более 1 элемента
	{                
	    split = (left + right) / 2;

		mergeSort(left, split, Comparator, temp);       // сортировать левую половину 
		mergeSort(split + 1, right, Comparator, temp);	// сортировать правую половину 
		merge(left, split, right, Comparator, temp);    // слить результаты в общий массив
	}
}
//==========================================================================================
template <class T> 
void OArray<T>::merge(int left, int split, int right, bool (*Comparator)(const T& a, const T& b), T* temp)
{
	// Слияние упорядоченных частей массива в буфер temp
	// с дальнейшим переносом содержимого temp в _begin[left]..._begin[right]

	// текущая позиция чтения из первой последовательности _begin[left]...a[split]
	int pos1 = left;

	// текущая позиция чтения из второй последовательности _begin[split+1]..._begin[right]
	int pos2 = split + 1;

	// текущая позиция записи в temp
	int pos3 = 0;  

	// идет слияние, пока есть хоть один элемент в каждой последовательности
	while (pos1 <= split && pos2 <= right)
	{
		if (Comparator(_begin[pos1], _begin[pos2]))
			temp[pos3++] = _begin[pos1++];
		else
			temp[pos3++] = _begin[pos2++];
	}

	// одна последовательность закончилась - 
	// копировать остаток другой в конец буфера 
	while (pos2 <= right)   // пока вторая последовательность непуста 
		temp[pos3++] = _begin[pos2++];
	while (pos1 <= split)  // пока первая последовательность непуста
		temp[pos3++] = _begin[pos1++];

	// скопировать буфер temp в a[lb]...a[ub]
	for (pos3 = 0; pos3 < right - left + 1; pos3++)
		_begin[left + pos3] = temp[pos3];
}

//==========================================================================================
template <class T>
void OArray<T>::group(bool (*Comparator)(const T& a, const T& b))
{
	// TODO: когда будет уверенность в group(lf,rt,Comparator), заменить этот код на
	// group(0, _size - 1, Comparator), чтобы не было копипасты
	int num = 0;
	for (int i = 0; i < _size; i += num)
	{
		num = 1;
		for (int j = i + 1; j < _size; j++)
		{
			if (Comparator(_begin[j], _begin[i]))
			{
				T tmp = _begin[j];
				for (int k = j; k > i + num; k--)
					_begin[k] = _begin[k-1];
				_begin[i+num] = tmp;
				num++;
			}
		}
	}
}

//==========================================================================================
template <class T>
void OArray<T>::group(int lf, int rt, bool (*Comparator)(const T& a, const T& b))
{
	int num = 0;
	for (int i = lf; i <= rt; i += num)
	{
		num = 1;
		for (int j = i + 1; j <= rt; j++)
		{
			if (Comparator(_begin[j], _begin[i]))
			{
				T tmp = _begin[j];
				for (int k = j; k > i + num; k--)
					_begin[k] = _begin[k-1];
				_begin[i+num] = tmp;
				num++;
			}
		}
	}
}

//==========================================================================================
template<class T>
void OArray<T>::groupToBegin(bool (*Predicate)(const T& a, const T& b))
{
	groupToBegin(0, _size - 1, Predicate);
}

//==========================================================================================
template<class T>
void OArray<T>::groupToBegin(int lf, int rt, bool (*Predicate)(const T& a, const T& b))
{
#ifdef	DEBUG
	abs_assert((lf <= rt && lf >= 0 && lf < _size && rt >= 0 && rt < _size) && "OArray<T>::groupToBegin() wrong bounds");
#endif
	int num = 1;
	for (int j = lf + 1; j <= rt; j++)
	{
		if (Predicate(_begin[j], _begin[lf]))
		{
			T tmp = _begin[j];
			for (int k = j; k > lf + num; k--)
				_begin[k] = _begin[k-1];
			_begin[lf+num] = tmp;
			num++;
		}
	}
}

//==========================================================================================
template <class T> void OArray<T>::shuffle()
{
	for (int index = 0; index < _size; index++)
	{
		int new_index = rand() % _size;
		if (new_index != index)
		{
			T temp = _begin[index];
			_begin[index] = _begin[new_index];
			_begin[new_index] = temp;
		}
	}
}
//==========================================================================================
// Сортировка пузырьком
//
template <class T> void OArray<T>::sortBubble(bool (*Less)(const T& a, const T& b))
{
	for (int i = _size - 2; i >= 0; i--)
	{
		T tmp;
		for (int j = 0; j <= i; j++)
		{
			if (Less(_begin[j + 1], _begin[j]))
			{
				tmp = _begin[j];
				_begin[j] = _begin[j+1];
				_begin[j+1] = tmp;
			}
		}
	}
	return;
}
//========================================================================================
//! установить все элементы в значение val
//
template<class T>  void   OArray<T>::set(const T & val)
{
   if(_size==0) return;                                         // на всякий случай защита
   T *c=begin(), *e=end();
   while(c<e) *c++=val;
}
//========================================================================================
//! установить все элементы между [lf..rt] в значение val
//
template<class T>  void   OArray<T>::set(const T& val, int lf, int rt)
{
   if(rt>=_size) rt=_size-1;                                     // на всякий случай защита
   for(T *c=_begin+lf, *end=_begin+rt+1; c<end; c++)
      *c=val;
}
//========================================================================================
// ищет первое вхождение в НЕ отсортированном массиве
//
template<class T> int OArray<T>::find(const T& val, int lf, int rt)
{
   if(rt>=_size) rt=_size-1;                                     // на всякий случай защита
   for(T *c=_begin+lf, *end=_begin+rt+1; c<end; c++)
      if( *c==val) return (c-_begin);
   return -1;
}
//========================================================================================
// ищет количество вхождений элемента в массив
//
template<class T> int OArray<T>::numOf(const T& val)
{
	int num = 0;
	for (int i = 0; i < _size; i++)
		if (_begin[i] == val)
			num++;
	return num;
}
//========================================================================================
// ищет первое вхождение в ОТСОРТИРОВАННОМ массиве
//
template<class T> int OArray<T>::findBinary(const T& val, int lf, int rt)
{
   if(rt>=_size) rt=_size-1;                                     // на всякий случай защита
   if(val<_begin[0]) return -1;                                 // int - 1 при rt=m-1
   while(rt >= lf){
      int m = (lf+rt)/2;
      if( val == _begin[m]) return m;
      if( val < _begin[m]) rt=m-1; else lf = m+1;
   }
   return -1;
}
//========================================================================================
// произвести преобразование с каждым элементы массива между [lf..rt] применяя к ним функцию Act
//
template<class T> void OArray<T>::transform(int lf, int rt, void (*Act)(T &a))
{
   if(rt>=_size) rt=_size-1;                                     // на всякий случай защита
   for(T *c=_begin+lf, *end=_begin+rt+1; c<end; c++)
      Act(*c);
}
//========================================================================================
// Просуммировать элементы массива между [lf..rt]
//
template<class T> T OArray<T>::sum(int lf, int rt)
{
   T sum=0;
   for(T *c=_begin+lf, *end=_begin+rt+1; c<end; )
      sum+=*c++;
   return sum;
}
//========================================================================================
// Просуммировать элементы массива по модулю module между [lf..rt]
//
template<class T> T OArray<T>::sumMod(int lf, int rt, const T module)
{
   T sum=0;
   for(T *c=_begin+lf, *end=_begin+rt+1; c<end; ){
      sum += *c++;
      sum = sum % module;
   }
   return sum;
}

//========================================================================================
// индексный доступ к элементам массива. 
//
template<class T> T &   OArray<T>::operator [](int i) 
{
	abs_assert((i>=0 && i<_size) && "OArray<T>::operator [] wrong index");
	return _begin[i];
} 

//========================================================================================
//  индексный доступ к элементам массива.
//
template<class T> const T& OArray<T>::operator [](int i) const 
{
	abs_assert((i>=0 && i<_size) && "OArray<T>::operator [] wrong index");
	return _begin[i];
}               
//========================================================================================

#endif
