/*
 * Copyright (c) 2014-2023, NVIDIA CORPORATION.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2014-2021 NVIDIA CORPORATION
 * SPDX-License-Identifier: Apache-2.0
 */

/* Contact chebert@nvidia.com (Chris Hebert) for feedback */

#ifndef __H_NODE_
#define __H_NODE_

#include "Renderable.h"
#include "Transform.h"
#include "Types.h"
#include <map>
#include <vector>


#pragma once
class Node
{
public:
  typedef size_t              ID;
  typedef size_t              Count;
  typedef std::map<ID, Node*> Map;
  typedef std::vector<Node*>  List;

  Node();
  Node(Node* inParent, const ID& inID);
  ~Node();

  void reset();
  bool update(bool inUpdateChildren = false);
  void draw();

  class NodeList
  {
  public:
    NodeList();
    ~NodeList();

    Node* newNode(const ID& inID, Node* inParent = NULL);
    void  addNode(Node* inNode, Node* inParent = NULL);
    Node* getNode(const ID& inID);

    Node::List& getData() { return m_data; }

    template <typename T>
    T* newNodeClass(Node* inParent = NULL)
    {

      typename T::ID id = nextID();

      T* node = new T(inParent, id);

      addNode(node);

      return node;
    }

    ID    nextID();
    Count count();

  private:
    Node::List m_data;
  };

  void setPosition(float inX, float inY, float inZ);
  void setRotation(float inX, float inY, float inZ);
  void setRotation(nvmath::quatf& inQuat);
  void setScale(float inX, float inY, float inZ);
  void setScale(float inScale);

  nvmath::vec4f worldPosition();
  nvmath::vec4f worldPosition(nvmath::vec4f& inPosition);

  Node* newChild();
  Node* newChild(const ID& inID);
  Node* newChild(const Vec4f& inPosition);
  Node* newChild(const float inX, const float inY, const float inZ);

  Renderable* newRenderable();
  Renderable* getRenderable(const Renderable::ID& inID);

  NodeList&  ChildNodes();
  Transform& GetTransform();

  Node* getParent() { return m_parent; }
  void  setParent(Node* inParent) { m_parent = inParent; }

  void getTriangles(render::TriangleList& outTriangles);

  ID getID() { return m_id; }

protected:
  ID        m_id     = 0;
  Node*     m_parent = nullptr;
  Transform m_transform;

  NodeList         m_child_nodes;
  Renderable::List m_renderables;

  nvmath::vec3f m_position{0.0f, 0.0f, 0.0f};
  nvmath::vec3f m_rotation{0.0f, 0.0f, 0.0f};
  nvmath::vec3f m_scale{1.0f, 1.0f, 1.0f};

  bool m_transform_needs_update = true;
};


#endif
