#pragma once

/// @brief ���I���X�g�N���X
template<class Ty>
class List {
	struct Node {
		Ty value;
		Node* next;
	};
	Node* first = nullptr;  /// �ŏ��̗v�f�������|�C���^
	Node* last = nullptr;   /// �Ō�̗v�f�������|�C���^

public:

	~List() {
		Node* current = first;
		while (current)
		{
			const auto temp = current;
			current = current->next;
			delete temp;
		}
	}

	inline List& operator<<(Ty&& r) {

		/// �V�����m�[�h���쐬
		if (first) {
			last->next = new Node{ r };
			last = last->next;
		}
		else {
			last = first = new Node{ r };
		}
		return *this;
	}

	/// @brief ���ׂĂ̗v�f�ɃA�N�Z�X
	/// @param f(T&) ���ꂼ��̗v�f�ɃA�N�Z�X����Ƃ��ɌĂ΂��֐�(�����ɗv�f�̎Q�Ƃ��n��)
	template<class... CapTy>
	inline void foreach(void (*f)(Ty&, CapTy&...), CapTy&&... capture) {
		Node* current = first;
		while (current) {
			f(current->value, capture...);
			current = current->next;
		}
	}

	List& operator=(List&& r) noexcept = delete;  /// ���������
};