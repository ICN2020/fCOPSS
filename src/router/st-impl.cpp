/*
  st-impl.cpp

  Copyright (c) 2019 KOZO KEIKAKU ENGINEERING Inc.

  This software is released under the MIT License

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "st-impl.hpp"

#include <fcopss/log.hpp>

namespace placeholders = boost::asio::placeholders;

namespace fcopss {
namespace router {

StImpl::StImpl(io_service& ioService, const time_duration& expireTime)
  : m_ioService(ioService), m_expireTime(expireTime)
{
}

std::tuple<bool, Cd>
StImpl::add(const Cd& cd, const FaceId& id)
{
  StExpiredCallback callback = boost::bind(&StImpl::onExpired, this, cd, id, placeholders::error);

  bool doForward = true;

  auto it = std::find_if(m_st.begin(), m_st.end(), [&](StEntry& entry) { return entry.m_cd == cd; });
  if (it != m_st.end()) {
    StEntry::NextHop nextHop(m_ioService, m_expireTime, callback, id);
    it->m_nextHops.erase(nextHop);
    it->m_nextHops.insert(nextHop);
    INFO("ST entry FaceID=%llu CD=%s already exists, update entry", id, cd.toUri().c_str());
  } else {
    StEntry::NextHop nextHop(m_ioService, m_expireTime, callback, id);
    StEntry entry(cd, nextHop);
    m_st.push_back(entry);
    INFO("ST entry FaceID=%llu CD=%s not exists, create entry", id, cd.toUri().c_str());
  }

  return std::make_tuple(doForward, cd);
}

void
StImpl::remove(const Cd& cd, const FaceId& id)
{
  auto it = std::find_if(m_st.begin(), m_st.end(), [&](StEntry& entry) { return entry.m_cd == cd; });
  if (it != m_st.end()) {
    StEntry::NextHop nextHop(id);
    if (it->m_nextHops.erase(nextHop) > 0) {
      INFO("ST entry FaceID=%llu CD=%s remoeved", id, cd.toUri().c_str());
    }
    if (it->m_nextHops.empty()) {
      m_st.erase(it);
    }
  }
}

void
StImpl::remove(const Cd& cd)
{
  auto it = std::find_if(m_st.begin(), m_st.end(), [&](StEntry& entry) { return entry.m_cd == cd; });
  if (it != m_st.end()) {
    m_st.erase(it);
    INFO("ST entry CD=%s remoeved", cd.toUri().c_str());
  }
}

void
StImpl::remove(const FaceId& id)
{
  StEntry::NextHop nextHop(id);

  auto it = m_st.begin();
  while (it != m_st.end()) {
    if (it->m_nextHops.erase(nextHop) > 0) {
      INFO("ST entry FaceID=%llu CD=%s remoeved", id, it->m_cd.toUri().c_str());
    }
    if (it->m_nextHops.empty()) {
      it = m_st.erase(it);
    } else {
      it++;
    }
  }
}

void
StImpl::clear()
{
  m_st.clear();
  INFO("all ST entry remoeved");
}

set<FaceId>
StImpl::match(const Cd& cd) const
{
  set<FaceId> faceIds;

  for (auto it = m_st.cbegin(); it != m_st.cend(); it++) {
    if (matchRegex(it->m_cd, cd)) {
      for (auto nextHop = it->m_nextHops.cbegin(); nextHop != it->m_nextHops.cend(); nextHop++) {
        faceIds.insert(nextHop->m_faceId);
        INFO("Packet CD=%s : ST entry match FaceID=%llu CD=%s",
             cd.toUri().c_str(), nextHop->m_faceId, it->m_cd.toUri().c_str());
      }
    }
  }

  if (faceIds.size() == 0) {
    INFO("Packet CD=%s : ST entry no match", cd.toUri().c_str());
  }

  return faceIds;
}

bool
StImpl::matchRegex(const Cd& entryCd, const Cd& inputCd)
{
  Block::element_const_iterator entryIt = entryCd.elements().cbegin();  
  Block::element_const_iterator inputIt = inputCd.elements().cbegin();  

  DEBUG("matching entry=%s : input=%s", entryCd.toUri().c_str(), inputCd.toUri().c_str());

  while (true) {
    if (entryIt == entryCd.elements().cend()) {
      DEBUG("entry end : stop");
      DEBUG("result = match");
      return true;
    }
    if (inputIt == inputCd.elements().cend()) {
      DEBUG("input end : stop");
      DEBUG("result = not match (1)");
      return false;
    }

    componentDebugPrint(*entryIt, *inputIt);

    if (entryIt->type() == tlv::CdAsterisk) {
      DEBUG("entry type = * : entry proceed");
      entryIt++;
    } else if (isEqualValue(*entryIt, *inputIt)) {
      DEBUG("entry value = input value : proceed both");
      entryIt++;
      inputIt++;
    } else if (entryIt->type() == tlv::CdOptional) {
      DEBUG("entry type = optional : proceed entry until normal component");
      do {
        entryIt++;
      } while (entryIt->type() != tlv::CdComponent) ;
    } else if (entryIt->type() == tlv::CdComponent) {
      DEBUG("entry type = normal component : stop");
      DEBUG("result = not match (2)");
      return false;
    }
  }
}

bool
StImpl::isEqualValue(const Block& entry, const Block& input)
{
  if (entry.value_size() != input.value_size()) {
    return false;
  }
  if (entry.value_size() > 0) {
    if (memcmp(entry.value(), input.value(), entry.value_size()) != 0) {
      return false;
    }
  }
  return true;
}

void
StImpl::componentDebugPrint(const Block& entry, const Block& input)
{
    char* p = getenv("FCOPSS_LOG_LEVEL");
    if ((p == nullptr) || (strcmp(p, "DEBUG"))) {
      return;
    }

    stringstream ss;

    if (entry.type() == tlv::CdAsterisk) {
      ss << "entry=* input=";
      for (size_t i = 0; i < input.value_size(); i++) {
        ss << *(input.value() + i);
      }
    } else {
      ss << "entry=";
      for (size_t i = 0; i < entry.value_size(); i++) {
        ss << *(entry.value() + i);
      }
      ss << ' ';
      ss << "input=";
      for (size_t i = 0; i < input.value_size(); i++) {
        ss << *(input.value() + i);
      }
    }

    DEBUG("%s", ss.str().c_str());
}

void
StImpl::dump(vector<string>& lines) const
{
  lines.clear();
  for (auto it = m_st.cbegin(); it != m_st.cend(); it++) {
    lines.push_back(it->toString());
  }
}

void
StImpl::onExpired(Cd cd, FaceId id, const boost::system::error_code& error)
{
  if (!error) {
    INFO("ST entry FaceID=%llu CD=%s expired", id, cd.toUri().c_str());
    remove(cd, id);
  } else {
  }
}

} // namespace router
} // namespace fcopss

