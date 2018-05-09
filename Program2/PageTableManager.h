/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageTableManager.h
 * Author: crjoyce
 *
 * Created on May 9, 2018, 4:20 PM
 */

#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H

#include <MMU.h>
class PageTableManager {
public:
    PageTableManager();
    PageTableManager(const PageTableManager& orig);
    virtual ~PageTableManager();
private:
    mem::PMCB *pmcb; 

};

#endif /* PAGETABLEMANAGER_H */

