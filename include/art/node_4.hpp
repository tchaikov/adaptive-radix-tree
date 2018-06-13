/**
 * @file node_4 header
 * @author Rafael Kallis <rk@rafaelkallis.com>
 */

#ifndef ART_NODE_4_HPP
#define ART_NODE_4_HPP

#include "node.hpp"
#include "node_16.hpp"

namespace art {

template <class T> class node_4 : public node<T> {
public:
  node_4();
  node_4(key_type prefix, T *value);
  node_4(const node_4<T> &other) = default;
  node_4(node_4<T> &&other) noexcept = default;
  ~node_4() override = default;

  node_4<T> &operator=(const node_4<T> &other) = default;
  node_4<T> &operator=(node_4<T> &&other) noexcept = default;

  node<T> *find_child(const partial_key_type &partial_key) const override;
  void set_child(const partial_key_type &partial_key, node<T> *child) override;
  node<T> *grow() override;
  bool is_full() const override;
  bool is_leaf() const override;

private:
  uint8_t n_children_;
  array<partial_key_type, 4> keys_;
  array<node<T> *, 4> children_;
};

template <class T> node_4<T>::node_4() : node_4<T>(key_type(0), nullptr) {}

template <class T>
node_4<T>::node_4(key_type prefix, T *value)
    : node<T>(prefix, value), n_children_(0), keys_(), children_() {}

template <class T>
node<T> *node_4<T>::find_child(const partial_key_type &partial_key) const {
  for (int i = 0; i < this->n_children_; i += 1) {
    if (this->keys_[i] == partial_key) {
      return this->children_[i];
    }
  }
  return nullptr;
}

template <class T>
void node_4<T>::set_child(const partial_key_type &partial_key, node<T> *child) {
  /* determine index for child */
  int child_i;
  for (int i = this->n_children_ - 1;; i -= 1) {
    if (i >= 0 && partial_key < this->keys_[i]) {
      /* move existing sibling to the right */
      this->keys_[i + 1] = this->keys_[i];
      this->children_[i + 1] = this->children_[i];
    } else {
      child_i = i + 1;
      break;
    }
  }

  this->keys_[child_i] = partial_key;
  this->children_[child_i] = child;
  this->n_children_ += 1;
}

template <class T> node<T> *node_4<T>::grow() {
  auto new_node = new node_16<T>(this->get_prefix(), this->get_value());
  for (int i = 0; i < this->n_children_; i += 1) {
    new_node->set_child(this->keys_[i], this->children_[i]);
  }
  delete this;
  return new_node;
}

template <class T> bool node_4<T>::is_full() const {
  return this->n_children_ == 4;
}

template <class T> bool node_4<T>::is_leaf() const {
  return this->n_children_ == 0;
}

} // namespace art

#endif