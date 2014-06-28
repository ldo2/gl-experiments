#ifndef GL_ARRAY_HPP__
#define GL_ARRAY_HPP__

template <typename ValueType, unsigned COMPONENTS = 1>
class GlArray {
public:
	typedef ValueType (Element)[COMPONENTS];
	typedef unsigned SizeType;

	GlArray(SizeType size) : 
			theData(new ValueType[size * COMPONENTS]), theSize(size) {
	}
	
	~GlArray() {
		delete[] theData;
	}
	
	GlArray(const GlArray&) = delete;
	GlArray &operator =(const GlArray&) = delete;
	
	SizeType size() const {
		return theSize;
	} 
	
	SizeType componentsSize() const {
		return theSize * COMPONENTS;
	}
	
	SizeType byteSize() const {
		return theSize * COMPONENTS * sizeof(ValueType);
	}
	
	ValueType *data() const {
		return theData;
	}
	
	Element& at(SizeType index) const {
		return *reinterpret_cast<Element*>(theData + index * COMPONENTS);
	}
	
private:
	ValueType *theData;
	SizeType theSize;
};

#endif // #GL_ARRAY_HPP__
