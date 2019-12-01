import React from 'react'
import { X, Plus } from 'react-feather'

import './index.less'

import MapWrapper from './components/MapWrapper/index'
import NormalCard from './components/NormalCard/index'

class Card extends React.Component {

  placeComponent () {
    const { info, changeState } = this.props
    switch(info.name) {
      case "map":
        if(info.isActive) {
          return <MapWrapper coord={info.data} />
        }
        return <div></div>
      case "heartRate":
        return <NormalCard type={info.name} bool={false} data={info.data} />
      case "fall":
      case "coGas":
        return <NormalCard type={info.name} bool={true}  data={info.data}/>
      case "alert":
        return <div>alert</div>
      default:
        return <div></div>
    }
  }

  render (){
    const { info, changeState } = this.props
    return (
      <div className="card__wrapper" style={{backgroundColor: (info.isActive)? '' : '#B2B2B2'}}> 
        <span className="card__wrapper--border" style={{borderColor: (!info.isActive) ? '#666666' : (info.isAlert) ? '#FF7B73' : ''}}>
          <div className="card__header">
            <span></span>
            <span> <h2>{info.text}</h2> </span>
            <span> <div onClick={() => {changeState(info.name)}}>{(info.isActive) ? <X size="40px" /> : <Plus size="40px" /> }</div> </span>
          </div>
          <div className="card__body">
            {this.placeComponent()}
          </div>
        </span>
      </div>
    )
  }
}

export default Card
