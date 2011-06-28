/**
 * @file
 * BusObject responsible for controlling/handling Bluetooth delegations.
 */

/******************************************************************************
 * Copyright 2009-2011, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/
#ifndef _ALLJOYN_BTNODEDB_H
#define _ALLJOYN_BTNODEDB_H

#include <qcc/platform.h>

#include <limits>
#include <set>
#include <vector>

#include <qcc/ManagedObj.h>
#include <qcc/Mutex.h>
#include <qcc/String.h>
#include <qcc/StringUtil.h>
#include <qcc/time.h>

#include "BDAddress.h"
#include "BTTransportConsts.h"
#include "Transport.h"


namespace ajn {

struct BTBusAddress {

    BDAddress addr;    /**< BDAddress part of the bus address. */
    uint16_t psm;      /**< L2CAP PSM part of the bus address. */

    /**
     * default constructor
     */
    BTBusAddress() : psm(bt::INVALID_PSM) { }

    /**
     * Copy constructor
     *
     * @param other     reference to other instance to intialize from
     */
    BTBusAddress(const BTBusAddress& other) : addr(other.addr), psm(other.psm) { }

    /**
     * Constructor that initializes from separate BDAddress and PSM.
     *
     * @param addr  BDAddress part of the bus address
     * @param psm   PSM part of the bus address
     */
    BTBusAddress(const BDAddress addr, uint16_t psm) : addr(addr), psm(psm) { }

    /**
     * Constructor that initializes from a bus address spec string.
     *
     * @param addrSpec  bus address encoded in a string: "bluetooth:addr=XX:XX:XX:XX:XX:XX,psm=0xXXXX"
     */
    BTBusAddress(const qcc::String& addrSpec) { FromSpec(addrSpec); }

    /**
     * The the bus address from a bus address spec string.
     *
     * @param addrSpec  bus address encoded in a string: "bluetooth:addr=XX:XX:XX:XX:XX:XX,psm=0xXXXX"
     */
    void FromSpec(const qcc::String& addrSpec)
    {
        std::map<qcc::String, qcc::String> argMap;
        Transport::ParseArguments("bluetooth", addrSpec.c_str(), argMap);
        addr.FromString(argMap["addr"]);
        psm = StringToU32(argMap["psm"], 0, bt::INVALID_PSM);
    }

    /**
     * Create a bus address spec string from the bus address.
     *
     * @return  a string representation of the bus address: "bluetooth:addr=XX:XX:XX:XX:XX:XX,psm=0xXXXX"
     */
    qcc::String ToSpec() const
    {
        return qcc::String("bluetooth:addr=" + addr.ToString() + ",psm=0x" + qcc::U32ToString(psm, 16, 4, '0'));
    }

    /**
     * Create a bus address string from the bus address in a human readable form.
     *
     * @return  a string representation of the bus address: "XX:XX:XX:XX:XX:XX-XXXX"
     */
    qcc::String ToString() const
    {
        return qcc::String(addr.ToString() + "-" + qcc::U32ToString(psm, 16, 4, '0'));
    }

    /**
     * Check is the bus address is valid.
     *
     * @return  true if the bus address is valid, false otherwise
     */
    bool IsValid() const { return psm != bt::INVALID_PSM; }

    /**
     * Less than operator.
     *
     * @param other     reference to the rhs of "<" for comparison
     *
     * @return  true if this is < other, false otherwise
     */
    bool operator<(const BTBusAddress& other) const
    {
        return (addr < other.addr) || ((addr == other.addr) && (psm < other.psm));
    }

    /**
     * Equivalence operator.
     *
     * @param other     reference to the rhs of "==" for comparison
     *
     * @return  true if this is == other, false otherwise
     */
    bool operator==(const BTBusAddress& other) const { return (addr == other.addr) && (psm == other.psm); }

    /**
     * Inequality operator.
     *
     * @param other     reference to the rhs of "==" for comparison
     *
     * @return  true if this is != other, false otherwise
     */
    bool operator!=(const BTBusAddress& other) const { return !(*this == other); }
};

/** Convenience typedef for a set of name strings. */
typedef std::set<qcc::String> NameSet;

/** Forward declaration of _BTNodeInfo. */
class _BTNodeInfo;

/** Typedef for BTNodeInfo ManagedObj. */
typedef qcc::ManagedObj<_BTNodeInfo> BTNodeInfo;

/** Class containing information about Bluetooth nodes. */
class _BTNodeInfo {

  public:
    /**
     * Default constructor.
     */
    _BTNodeInfo() :
        guid(),
        uniqueName(),
        nodeAddr(),
        directMinion(false),
        connectProxyNode(NULL),
        uuidRev(bt::INVALID_UUIDREV),
        expireTime(std::numeric_limits<uint64_t>::max())
    { }

    /**
     * Construct that initializes certain information.
     *
     * @param nodeAddr      Bus address of the node
     */
    _BTNodeInfo(const BTBusAddress& nodeAddr) :
        guid(),
        uniqueName(),
        nodeAddr(nodeAddr),
        directMinion(false),
        connectProxyNode(NULL),
        uuidRev(bt::INVALID_UUIDREV),
        expireTime(std::numeric_limits<uint64_t>::max())
    { }

    /**
     * Construct that initializes certain information.
     *
     * @param nodeAddr      Bus address of the node
     * @param uniqueName    Unique bus name of the daemon on the node
     */
    _BTNodeInfo(const BTBusAddress& nodeAddr, const qcc::String& uniqueName) :
        guid(),
        uniqueName(uniqueName),
        nodeAddr(nodeAddr),
        directMinion(false),
        connectProxyNode(NULL),
        uuidRev(bt::INVALID_UUIDREV),
        expireTime(std::numeric_limits<uint64_t>::max())
    { }

    /**
     * Construct that initializes certain information.
     *
     * @param nodeAddr      Bus address of the node
     * @param uniqueName    Unique bus name of the daemon on the node
     * @param guid          Bus GUID of the node
     */
    _BTNodeInfo(const BTBusAddress& nodeAddr, const qcc::String& uniqueName, const qcc::String& guid) :
        guid(guid),
        uniqueName(uniqueName),
        nodeAddr(nodeAddr),
        directMinion(false),
        connectProxyNode(NULL),
        uuidRev(bt::INVALID_UUIDREV),
        expireTime(std::numeric_limits<uint64_t>::max())
    { }

    /**
     * Destructor.
     */
    ~_BTNodeInfo() { if (connectProxyNode) { delete connectProxyNode; } }

    /**
     * Check is the node information is valid.
     *
     * @return  true if the node information valid, false otherwise
     */
    bool IsValid() const { return nodeAddr.IsValid(); }

    /**
     * Get the begin iterator for the advertise name set.
     *
     * @return  const_iterator pointing to the begining of the advertise name set
     */
    NameSet::const_iterator GetAdvertiseNamesBegin() const { return adNames.begin(); }

    /**
     * Get the end iterator for the advertise name set.
     *
     * @return  const_iterator pointing to the end of the advertise name set
     */
    NameSet::const_iterator GetAdvertiseNamesEnd() const { return adNames.end(); }

    /**
     * Get the iterator pointing to the specified name for the advertise name set.
     *
     * @param name  Name of the advertise name to find
     *
     * @return  const_iterator pointing to the specified name or the end of the advertise name set
     */
    NameSet::const_iterator FindAdvertiseName(const qcc::String& name) const { return adNames.find(name); }

    /**
     * Get the begin iterator for the advertise name set.
     *
     * @return  const_iterator pointing to the begining of the advertise name set
     */
    NameSet::iterator GetAdvertiseNamesBegin() { return adNames.begin(); }

    /**
     * Get the end iterator for the advertise name set.
     *
     * @return  const_iterator pointing to the end of the advertise name set
     */
    NameSet::iterator GetAdvertiseNamesEnd() { return adNames.end(); }

    /**
     * Get the iterator pointing to the specified name for the advertise name set.
     *
     * @param name  Name of the advertise name to find
     *
     * @return  const_iterator pointing to the specified name or the end of the advertise name set
     */
    NameSet::iterator FindAdvertiseName(const qcc::String& name) { return adNames.find(name); }

    /**
     * Get the number of entries in the advertise name set.
     *
     * @return  the number of entries in the advertise name set
     */
    size_t AdvertiseNamesSize() const { return adNames.size(); }

    /**
     * Check if the advertise name set is empty
     *
     * @return  true if the advertise name set is empty, false otherwise
     */
    bool AdvertiseNamesEmpty() const { return adNames.empty(); }

    /**
     * Add a name to the advertise name set.
     *
     * @param name  Name to add to the advertise name set
     */
    void AddAdvertiseName(const qcc::String& name) { adNames.insert(name); }

    /**
     * Remove a name from the advertise name set.
     *
     * @param name  Name to remove from the advertise name set
     */
    void RemoveAdvertiseName(const qcc::String& name) { adNames.erase(name); }

    /**
     * Remove a name from the advertise name set referenced by an iterator.
     *
     * @param it    Reference to the name to remove from the advertise name set
     */
    void RemoveAdvertiseName(NameSet::iterator& it) { adNames.erase(it); }


    /**
     * Get the begin iterator for the find name set.
     *
     * @return  const_iterator pointing to the begining of the find name set
     */
    NameSet::const_iterator GetFindNamesBegin() const { return findNames.begin(); }

    /**
     * Get the end iterator for the find name set.
     *
     * @return  const_iterator pointing to the end of the find name set
     */
    NameSet::const_iterator GetFindNamesEnd() const { return findNames.end(); }

    /**
     * Get the iterator pointing to the specified name for the find name set.
     *
     * @param name  Name of the find name to find
     *
     * @return  const_iterator pointing to the specified name or the end of the find name set
     */
    NameSet::const_iterator FindFindName(const qcc::String& name) const { return findNames.find(name); }

    /**
     * Get the begin iterator for the find name set.
     *
     * @return  const_iterator pointing to the begining of the find name set
     */
    NameSet::iterator GetFindNamesBegin() { return findNames.begin(); }

    /**
     * Get the end iterator for the find name set.
     *
     * @return  const_iterator pointing to the end of the find name set
     */
    NameSet::iterator GetFindNamesEnd() { return findNames.end(); }

    /**
     * Get the iterator pointing to the specified name for the find name set.
     *
     * @param name  Name of the find name to find
     *
     * @return  const_iterator pointing to the specified name or the end of the find name set
     */
    NameSet::iterator FindFindName(const qcc::String& name) { return findNames.find(name); }

    /**
     * Get the number of entries in the findadvertise name set.
     *
     * @return  the number of entries in the find name set
     */
    size_t FindNamesSize() const { return findNames.size(); }

    /**
     * Check if the find name set is empty
     *
     * @return  true if the find name set is empty, false otherwise
     */
    bool FindNamesEmpty() const { return findNames.empty(); }

    /**
     * Add a name to the find name set.
     *
     * @param name  Name to add to the find name set
     */
    void AddFindName(const qcc::String& name) { findNames.insert(name); }

    /**
     * Remove a name from the find name set.
     *
     * @param name  Name to remove from the find name set
     */
    void RemoveFindName(const qcc::String& name) { findNames.erase(name); }

    /**
     * Remove a name from the find name set referenced by an iterator.
     *
     * @param it    Reference to the name to remove from the find name set
     */
    void RemoveFindName(NameSet::iterator& it) { findNames.erase(it); }

    /**
     * Get the bus GUID.
     *
     * @return  String representation of the bus GUID.
     */
    const qcc::String& GetGUID() const { return guid; }

    /**
     * Set the bus GUID.
     *
     * @param guid  String representation of the bus GUID.
     */
    void SetGUID(const qcc::String& guid) { this->guid = guid; }

    /**
     * Get the unique name of the AllJoyn controller object.
     *
     * @return  The unique name of the AllJoyn controller object.
     */
    const qcc::String& GetUniqueName() const { return uniqueName; }

    /**
     * Set the unique name of the AllJoyn controller object.  Care must be
     * taken when setting this.  It is used as a lookup key in BTNodeDB and
     * setting this for a node contained by BTNodeDB will _NOT_ update that
     * index.
     *
     * @param name  The unique name of the AllJoyn controller object.
     */
    void SetUniqueName(const qcc::String& name) { uniqueName = name; }

    /**
     * Get the Bluetooth bus address.
     *
     * @return  The Bluetooth bus address.
     */
    const BTBusAddress& GetBusAddress() const { return nodeAddr; }

    /**
     * Set the Bluetooth bus address.  Care must be taken when setting this.
     * It is used as a lookup key in BTNodeDB and setting this for a node
     * contained by BTNodeDB will _NOT_ update that index.
     *
     * @param addr  The Bluetooth bus address.
     */
    void SetBusAddress(const BTBusAddress& addr) { nodeAddr = addr; }

    /**
     * Get whether this node is a direct minion or not.
     *
     * @return  'true' if the node is a direct minion, 'false' otherwise.
     */
    bool IsDirectMinion() const { return directMinion; }

    /**
     * Set whether this node is a direct minion or not.
     *
     * @param val   'true' if the node is a direct minion, 'false' otherwise.
     */
    void SetDirectMinion(bool val) { directMinion = val; }

    /**
     * Get the bus address that is accepting connections for us.
     *
     * @return  Bus address accepting connections for us
     */
    const BTBusAddress& GetConnectAddress() const
    {
        const _BTNodeInfo* next = this;
        while (next->connectProxyNode) {
            next = &(*(*(next->connectProxyNode)));
        }
        return next->GetBusAddress();
    }

    /**
     * Set the node that accepts connections for us.  It may actually have
     * node that accepts connections for it, thus producing a chain.  Care
     * must be taken when setting this.  It is used as a lookup key in
     * BTNodeDB and setting this for a node contained by BTNodeDB will _NOT_
     * update that index.
     *
     * @param node  Node that will accept connections for us.
     */
    void SetConnectNode(const BTNodeInfo& node)
    {
        if (*node == *this) {
            if (connectProxyNode) {
                delete connectProxyNode;
                connectProxyNode = NULL;
            } // connectProxyNode == NULL -- nothing to do
        } else {
            if (connectProxyNode) {
                *connectProxyNode = node;
            } else {
                connectProxyNode = new BTNodeInfo(node);
            }
        }
    }

    /**
     * Get the UUID revision of the advertisement this node was discovered in.
     *
     * @return  The UUID revision.
     */
    uint32_t GetUUIDRev() const { return uuidRev; }

    /**
     * Set the UUID revision of the advertisement this node was discovered in.
     *
     * @param uuidRev   The UUID revision.
     */
    void SetUUIDRev(uint32_t uuidRev) { this->uuidRev = uuidRev; }

    /**
     * Get the absolute expire time in milliseconds.  If value is
     * numeric_limits<uint64_t>::max() then no expiration timeout set.
     *
     * @return  Absolute expiration time in milliseconds
     */
    uint64_t GetExpireTime() const { return expireTime; }

    /**
     * Set the expiration time.  Care must be taken when setting this.  It is
     * used as a lookup key in BTNodeDB and setting this for a node contained
     * by BTNodeDB will _NOT_ update that index.
     *
     * @param expireTime    Absolute expiration time in milliseconds
     */
    void SetExpireTime(uint64_t expireTime)
    {
        this->expireTime = expireTime;
    }

    /**
     * Equivalence operator.
     *
     * @param other     reference to the rhs of "==" for comparison
     *
     * @return  true if this is == other, false otherwise
     */
    bool operator==(const _BTNodeInfo& other) const { return (nodeAddr == other.nodeAddr); }

    /**
     * Inequality operator.
     *
     * @param other     reference to the rhs of "==" for comparison
     *
     * @return  true if this is != other, false otherwise
     */
    bool operator!=(const _BTNodeInfo& other) const { return !(*this == other); }

    /**
     * Less than operator.
     *
     * @param other     reference to the rhs of "<" for comparison
     *
     * @return  true if this is < other, false otherwise
     */
    bool operator<(const _BTNodeInfo& other) const { return (nodeAddr < other.nodeAddr); }

  private:
    /**
     * Private copy constructor to catch potential coding errors.
     */
    _BTNodeInfo(const _BTNodeInfo& other) { }

    /**
     * Private assignment operator to catch potential coding errors.
     */
    _BTNodeInfo& operator=(const _BTNodeInfo& other) { return *this; }

    qcc::String guid;             /**< Bus GUID of the node. */
    qcc::String uniqueName;       /**< Unique bus name of the daemon on the node. */
    BTBusAddress nodeAddr;        /**< Bus address of the node. */
    bool directMinion;            /**< Flag indicating if the node is a directly connected minion or not. */
    BTNodeInfo* connectProxyNode; /**< Node that will accept connections for us. */
    NameSet adNames;              /**< Set of advertise names. */
    NameSet findNames;            /**< Set of find names. */
    uint32_t uuidRev;             /**< UUID revision of the advertisement this node was found in. */
    uint64_t expireTime;          /**< Time when advertised information is considered stale. */
};


/** Bluetooth Node Database */
class BTNodeDB {
  public:
    /** Convenience iterator typedef. */
    typedef std::set<BTNodeInfo>::iterator iterator;

    /** Convenience const_iterator typedef. */
    typedef std::set<BTNodeInfo>::const_iterator const_iterator;

    /**
     * Find a node given a Bluetooth device address and a PSM.
     *
     * @param addr  Bluetooth device address
     * @param psm   L2CAP PSM for the AllJoyn service
     *
     * @return  BTNodeInfo of the found node (BTNodeInfo::IsValid() will return false if not found)
     */
    const BTNodeInfo FindNode(const BDAddress& addr, uint16_t psm) const { BTBusAddress busAddr(addr, psm); return FindNode(busAddr); }

    /**
     * Find a node given a bus address.
     *
     * @param addr  bus address
     *
     * @return  BTNodeInfo of the found node (BTNodeInfo::IsValid() will return false if not found)
     */
    const BTNodeInfo FindNode(const BTBusAddress& addr) const;

    /**
     * Find a node given a unique name of the daemon running on a node.
     *
     * @param uniqueName    unique name of the daemon running on a node
     *
     * @return  BTNodeInfo of the found node (BTNodeInfo::IsValid() will return false if not found)
     */
    const BTNodeInfo FindNode(const qcc::String& uniqueName) const;

    /**
     * Find the first node with given a Bluetooth device address.  (Generally,
     * the bluetooth device address should be unique, but it is not completely
     * impossible for 2 instances for AllJoyn to be running on one physical
     * device with the same Bluetooth device address but with different PSMs.)
     *
     * @param addr  Bluetooth device address
     *
     * @return  BTNodeInfo of the found node (BTNodeInfo::IsValid() will return false if not found)
     */
    const BTNodeInfo FindNode(const BDAddress& addr) const;

    /**
     * Find a direct minion starting with the specified start node in the set
     * of nodes, and skipping over the skip node.
     *
     * @param start     Node in the DB to use as a starting point for the search
     * @param skip      Node in the DB to skip over if next in line
     *
     * @return  BTNodeInfo of the next direct minion.  Will be the same as start if none found.
     */
    BTNodeInfo FindDirectMinion(const BTNodeInfo& start, const BTNodeInfo& skip) const;

    /**
     * Add a node to the DB with no expiration time.
     *
     * @param node  Node to be added to the DB.
     */
    void AddNode(const BTNodeInfo& node);

    /**
     * Remove a node from the DB.
     *
     * @param node  Node to be removed from the DB.
     */
    void RemoveNode(const BTNodeInfo& node);

    /**
     * Determine the difference between this DB and another DB.  Nodes that
     * appear in only one or the other DB will be copied (i.e., share the same
     * referenced data) to the added/removed DBs as appropriate.  Nodes that
     * appear in both this and the other DB but have differences in their set
     * of advertised names will result in fully independed copies of the node
     * information with only the appropriate name changes being put in the
     * added/removed DBs as appropriate.  It is possible for a node to appear
     * in both the added DB and removed DB if that node had advertised names
     * both added and removed.
     *
     * @param other         Other DB for comparision
     * @param added[out]    If non-null, the set of nodes (and names) found in
     *                      other but not in us
     * @param removed[out]  If non-null, the set of nodes (and names) found in
     *                      us but not in other
     */
    void Diff(const BTNodeDB& other, BTNodeDB* added, BTNodeDB* removed) const;

    /**
     * Determine the difference between this DB and another DB in terms of
     * nodes only.  In other words, nodes in this DB that do not appear in the
     * other DB will be copied into the removed DB while nodes in the other DB
     * that do not appear in this DB will be copied to the added DB.
     * Differences in names will be ignored.
     *
     * @param other         Other DB for comparision
     * @param added[out]    If non-null, the set of nodes found in other but not in us
     * @param removed[out]  If non-null, the set of nodes found in us but not in other
     */
    void NodeDiff(const BTNodeDB& other, BTNodeDB* added, BTNodeDB* removed) const;

    /**
     * Applies the differences found in BTNodeDB::Diff to us.
     *
     * @param added         If non-null, nodes (and names) to add
     * @param removed       If non-null, name to remove from nodes
     * @param removeNodes   Optional parameter that defaults to true
     *                      - true: remove nodes that become empty due to all
     *                              names being removed
     *                      - false: keep empty nodes
     */
    void UpdateDB(const BTNodeDB* added, const BTNodeDB* removed, bool removeNodes = true);

    /**
     * Removes the expiration time of all nodes (sets expiration to end-of-time).
     */
    void RemoveExpiration();

    /**
     * Updates the expiration time of all nodes.
     *
     * @param expireDelta   Number of milliseconds from now to set the
     *                      expiration time.
     */
    void RefreshExpiration(uint32_t expireDelta);

    /**
     * Updates the expiration time of all nodes that may be connected to via
     * connAddr.
     *
     * @param connAddr      BusAddress of the device accepting connections on
     *                      behalf of other nodes.
     * @param expireDelta   Number of milliseconds from now to set the
     *                      expiration time.
     */
    void RefreshExpiration(const BTBusAddress& connAddr, uint32_t expireDelta);

    /**
     * Updates the expiration time of the specified node.
     *
     * @param node          Node to refresh the expiration for.
     * @param expireDelta   Number of milliseconds from now to set the
     *                      expiration time.
     */
    void RefreshExpiration(const BTNodeInfo& node, uint32_t expireDelta);

    /**
     * Fills a BTNodeDB with the set of nodes that are connectable via
     * connAddr.
     *
     * @param connAddr  BusAddress of the device accepting connections on
     *                  behalf of other nodes.
     * @param subDB     Sub-set BTNodeDB to store the found nodes in.
     */
    void GetNodesFromConnectAddr(const BTBusAddress& connAddr, BTNodeDB& subDB) const
    {
        ConnAddrMap::const_iterator cmit = connMap.lower_bound(connAddr);
        ConnAddrMap::const_iterator end = connMap.upper_bound(connAddr);

        while (cmit != end) {
            subDB.AddNode(cmit->second);
            ++cmit;
        }
    }

    void PopExpiredNodes(BTNodeDB& expiredDB)
    {
        Lock();
        qcc::Timespec now;
        qcc::GetTimeNow(&now);
        while (!expireSet.empty() && ((*expireSet.begin())->GetExpireTime() <= now.GetAbsoluteMillis())) {
            BTNodeInfo node = *expireSet.begin();
            RemoveNode(node);
            expiredDB.AddNode(node);
        }
        Unlock();
    }

    uint64_t NextNodeExpiration()
    {
        if (!expireSet.empty()) {
            return (*expireSet.begin())->GetExpireTime();
        }
        return std::numeric_limits<uint64_t>::max();
    }

    /**
     * Lock the mutex that protects the database from unsafe access.
     */
    void Lock() const { lock.Lock(); }

    /**
     * Release the the mutex that protects the database from unsafe access.
     */
    void Unlock() const { lock.Unlock(); }

    /**
     * Get the begin iterator for the set of nodes.
     *
     * @return  const_iterator pointing to the first node
     */
    const_iterator Begin() const { return nodes.begin(); }

    /**
     * Get the end iterator for the set of nodes.
     *
     * @return  const_iterator pointing to one past the last node
     */
    const_iterator End() const { return nodes.end(); }

    /**
     * Get the number of entries in the node DB.
     *
     * @return  the number of entries in the node DB
     */
    size_t Size() const
    {
        Lock();
        size_t size = nodes.size();
        Unlock();
        return size;
    }

    /**
     * Clear out the DB.
     */
    void Clear() { nodes.clear(); addrMap.clear(); nameMap.clear(); connMap.clear(); expireSet.clear(); }

#ifndef NDEBUG
    void DumpTable(const char* info) const;
#else
    void DumpTable(const char* info) const { }
#endif


  private:

    class ExpireNodeComp {
      public:
        bool operator()(const BTNodeInfo& lhs, const BTNodeInfo& rhs) const
        {
            return ((lhs->GetExpireTime() < rhs->GetExpireTime()) ||
                    ((lhs->GetExpireTime() == rhs->GetExpireTime()) && (lhs < rhs)));
        }
    };

    /** Convenience typedef for the lookup table keyed off the bus address. */
    typedef std::map<BTBusAddress, BTNodeInfo> NodeAddrMap;

    /** Convenience typedef for the lookup table keyed off the bus address. */
    typedef std::multimap<BTBusAddress, BTNodeInfo> ConnAddrMap;

    /** Convenience typedef for the lookup table keyed off the unique bus name. */
    typedef std::map<qcc::String, BTNodeInfo> NodeNameMap;

    /** Convenience typedef for the lookup table sorted by expiration time. */
    typedef std::set<BTNodeInfo, ExpireNodeComp> NodeExpireSet;

    /** Convenience typedef for the lookup table sorted by connect address. */

    std::set<BTNodeInfo> nodes;     /**< The node DB storage. */
    NodeAddrMap addrMap;            /**< Lookup table keyed off the bus address. */
    NodeNameMap nameMap;            /**< Lookup table keyed off the unique bus name. */
    NodeExpireSet expireSet;        /**< Lookup table sorted by the expiration time. */
    ConnAddrMap connMap;         /**< Lookup table keyed off the connect node. */

    mutable qcc::Mutex lock;        /**< Mutext to protect the DB. */

};

} // namespace ajn

#endif
