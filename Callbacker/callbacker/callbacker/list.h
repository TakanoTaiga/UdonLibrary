#pragma once

/// @brief �P�������X�g
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
		while (current) {
			const auto temp = current;
			current = current->next;
			delete temp;
		}
	}

	/// @brief �V�����v�f��ǉ�����
	inline List& operator<<(Ty&& r) {
		if (first)
			last = last->next = new Node{ r };
		else
			last = first = new Node{ r };
		return *this;
	}

	/// @brief ���ׂĂ̗v�f�ɃA�N�Z�X
	/// @param CapTy �Ă΂��֐��ɓn������
	/// @param f(T&) ���ꂼ��̗v�f�ɃA�N�Z�X����֐�(�����ɗv�f�̎Q�Ƃ��n��)
	/// @param capture �A�N�Z�b�T�֐��ɓn������
	template<class... CapTy>
	inline void foreach(void (*f)(Ty&, CapTy&...), CapTy&&... capture) {
		Node* current = first;
		while (current) {
			f(current->value, static_cast<CapTy&>(capture)...);
			current = current->next;
		}
	}

	List& operator=(List&& r) noexcept = delete;  /// ���������
};